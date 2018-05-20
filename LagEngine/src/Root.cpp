#include "Root.h"

#include <tinyxml/tinyxml.h>

#include "platform/GLFW_GL4Factory.h"

#include "InitializationParameters.h"
#include "io/log/LogManager.h"

#include "renderer/Renderer.h"
#include "renderer/RenderWindow.h"
#include "renderer/graphicsAPI/IGraphicsAPI.h"
#include "renderer/graphicsAPI/GpuProgramManager.h"
#include "renderer/graphicsAPI/GpuBufferManager.h"
#include "renderer/graphicsAPI/InputDescriptionManager.h"

#include "io/InputManager.h"
#include "scene/SceneManager.h"

#include "resources/MaterialManager.h"
#include "resources/MeshManager.h"
#include "resources/ImageManager.h"
#include "resources/TextureManager.h"
#include "resources/GpuProgramStageManager.h"

#include "io/Keys.h"

#ifdef _DEBUG
	#include "io/ResourceFilesWatcher.h"
#endif

#include "Constants.h"

using namespace Lag;

Root::Root() :
	inputManager(nullptr),
	renderer(nullptr),
	sceneManager(nullptr),
	renderTargetManager(nullptr),
	gpuProgramStageManager(nullptr),
	gpuProgramManager(nullptr),
	materialManager(nullptr),
	meshManager(nullptr),
	imageManager(nullptr),
	textureManager(nullptr),
	gpuBufferManager(nullptr),
	graphicsAPI(nullptr),
	inputDescriptionManager(nullptr),
	appResourcesFile(nullptr),
	lagResourcesFile(nullptr)
{
	//Initialize other singletons
	LogManager::getInstance();

#ifdef _DEBUG
	resourceFilesWatcher = nullptr;
#endif
}

Root::~Root()
{
	destroy();
}

void Root::destroy()
{
	if (sceneManager != nullptr)
	{
		delete sceneManager;
		sceneManager = nullptr;
	}
	
	if (materialManager != nullptr)
	{
		delete materialManager;
		materialManager = nullptr;
	}
	if (gpuProgramManager != nullptr)
	{
		delete gpuProgramManager;
		gpuProgramManager = nullptr;
	}
	if (gpuProgramStageManager != nullptr)
	{
		delete gpuProgramStageManager;
		gpuProgramStageManager = nullptr;
	}
	if (textureManager != nullptr)
	{
		delete textureManager;
		textureManager = nullptr;
	}
	if (imageManager != nullptr)
	{
		delete imageManager;
		imageManager = nullptr;
	}
	if (meshManager != nullptr)
	{
		delete meshManager;
		meshManager = nullptr;
	}

	if (gpuBufferManager != nullptr)
	{
		delete gpuBufferManager;
		gpuBufferManager = nullptr;
	}
	if (inputDescriptionManager != nullptr)
	{
		delete inputDescriptionManager;
		inputDescriptionManager = nullptr;
	}

	if (graphicsAPI != nullptr)
	{
		delete graphicsAPI;
		graphicsAPI = nullptr;
	}
	if (renderer != nullptr)
	{
		delete renderer;
		renderer = nullptr;
	}

	if (inputManager != nullptr)
	{
		delete inputManager;
		inputManager = nullptr;
	}

	if (appResourcesFile != nullptr)
	{
		delete appResourcesFile;
		appResourcesFile = nullptr;
	}
	if (lagResourcesFile != nullptr)
	{
		delete lagResourcesFile;
		lagResourcesFile = nullptr;
	}

	if (renderTargetManager != nullptr)
	{
		delete renderTargetManager;
		renderTargetManager = nullptr;
	}

#ifdef _DEBUG
	if (resourceFilesWatcher != nullptr)
	{
		delete resourceFilesWatcher;
		resourceFilesWatcher = nullptr;
	}
#endif
}

bool Root::initializeLag(const std::string &iniFile)
{
	initializationParameters = InitializationParameters(iniFile);
	
	IPlatformFactory *platformFactory = nullptr;
	if (initializationParameters.platformType == LAG_PLATFORM_GLFW_GL4)
		platformFactory = new GLFW_GL4Factory();

	if (platformFactory == nullptr)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Cannot find a suitable platform factory. Double check the choosen platform on the .ini file.");
		return false;
	}
	else
	{
		bool result = internalInit(platformFactory);
		delete platformFactory;
		return result;
	}
}

bool Root::internalInit(const IPlatformFactory *platformFactory)
{
	//in case of reinitialization
	destroy();

	renderTargetManager = platformFactory->getRenderTargetManager();
	renderWindow = renderTargetManager->getRenderWindow(initializationParameters);
	if (!renderWindow.isValid())
		return false;

	inputManager = platformFactory->getInputManager(static_cast<RenderWindow&>(*renderWindow));

	graphicsAPI = platformFactory->getGraphicsAPI();
	if (!graphicsAPI->initialize())
		return false;

	gpuBufferManager = platformFactory->getGpuBufferManager();
	gpuProgramManager = platformFactory->getGpuProgramManager();
	inputDescriptionManager = platformFactory->getInputDescriptionManager();

	if (!initResources(platformFactory))
		return false;

	sceneManager = new SceneManager();
	renderer = new Renderer(*graphicsAPI, *sceneManager, *renderTargetManager);
	sceneManager->registerObservers(); //Needs to be called after Renderer creation

	static_cast<RenderWindow*>(renderWindow.get())->registerObserver(windowListener);
	inputManager->registerObserver(keyboardListener);

#ifdef _DEBUG
	resourceFilesWatcher = new ResourceFilesWatcher(initializationParameters);
#endif
	
	return true;
}

bool Root::initResources(const IPlatformFactory *platformFactory)
{
	if (!initResourcesFiles())
		return false;

	textureManager = platformFactory->getTextureManager(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile, "", "", TEXTURE_XML_TAG));

	gpuProgramStageManager = platformFactory->getGpuProgramStageManager(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile, 
		initializationParameters.getShadersFolder(false), initializationParameters.getShadersFolder(true),
		SHADER_XML_TAG));

	imageManager = new ImageManager(new ImageBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile, 
		initializationParameters.getImagesFolder(false), initializationParameters.getImagesFolder(true), 
		IMAGE_XML_TAG)));

	meshManager = new MeshManager(new MeshBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile, 
		initializationParameters.getMeshesFolder(false), initializationParameters.getMeshesFolder(true),
		MESH_XML_TAG)));

	materialManager = new MaterialManager(new MaterialBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile,
		initializationParameters.getMaterialsFolder(false), initializationParameters.getMaterialsFolder(true),
		MATERIAL_XML_TAG)));

	imageManager->initializeFallbackObject();
	gpuProgramManager->initializeFallbackObject();
	textureManager->initializeFallbackObject();
	materialManager->initializeFallbackObject();
	meshManager->initializeFallbackObject();

	return true;
}

bool Root::initResourcesFiles()
{
	std::string lagResourcesFilePath = initializationParameters.lagResourcesFolder + '/' + initializationParameters.resourcesFile;
	std::string resourcesFilePath = initializationParameters.appResourcesFolder + '/' + initializationParameters.resourcesFile;

	TiXmlDocument *lagResourcesFile = new TiXmlDocument(lagResourcesFilePath);
	if (!checkResourcesFile(lagResourcesFilePath, lagResourcesFile))
	{
		delete lagResourcesFile;
		return false;
	}

	TiXmlDocument *appResourcesFile = new TiXmlDocument(resourcesFilePath);
	if (!checkResourcesFile(resourcesFilePath, appResourcesFile))
	{
		delete lagResourcesFile;
		delete appResourcesFile;
		return false;
	}

	this->appResourcesFile = appResourcesFile;
	this->lagResourcesFile = lagResourcesFile;
	return true;
}

bool Root::checkResourcesFile(const std::string &filePath, TiXmlDocument *resourcesFile) const
{
	if (!resourcesFile->LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + filePath + " does not exist or is malformed.");
		return false;
	}

	const TiXmlElement *resourcesElement = resourcesFile->FirstChildElement();
	if (!resourcesElement)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + filePath + " does not contain <resources> element.");
		return false;
	}

	return true;
}

void Root::reloadResourcesFile()
{
	auto oldAppResourcesFile = appResourcesFile;
	auto oldLagResourcesFile = lagResourcesFile;
	if (initResourcesFiles())
	{
		delete oldAppResourcesFile;
		delete oldLagResourcesFile;
	}
}

void Root::startRenderingLoop()
{
	renderer->startRenderingLoop(initializationParameters.maxFPS);
}

void Root::stopRenderingLoop()
{
	renderer->stopRenderingLoop();
}

void Root::renderOneFrame()
{
	renderer->renderOneFrame();
}

void Root::KeyboardListener::onKeyPress(int key, int modifier)
{
	switch (key)
	{
	case KEY_LEFT_CONTROL: 
	{
		RenderWindow *renderWindow = Root::getInstance().getRenderTargetManager().getRenderWindow();
		renderWindow->setVirtualCursor(!renderWindow->isVirtualCursorEnabled());
	}
		break;
	case KEY_ESCAPE:
		Root::getInstance().stopRenderingLoop();
		break;
	default:
		break;
	}
}

void Root::WindowListener::onClose(RenderWindow &notifier)
{
	Root::getInstance().stopRenderingLoop();
}