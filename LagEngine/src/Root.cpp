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
	resourcesFile(nullptr)
{
	//Initialize other singletons
	LogManager::getInstance();
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

	if (resourcesFile != nullptr)
	{
		delete resourcesFile;
		resourcesFile = nullptr;
	}

	if (renderTargetManager != nullptr)
	{
		delete renderTargetManager;
		renderTargetManager = nullptr;
	}
}

bool Root::initializeLag(const std::string &iniFile)
{
	initializationParameters = InitializationParameters(iniFile);
	return initializeLag();
}

bool Root::initializeLag()
{
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

	if (!initResources(platformFactory, initializationParameters.resourcesFolder + '/' + initializationParameters.resourcesFile))
		return false;

	sceneManager = new SceneManager();
	renderer = new Renderer(*graphicsAPI, *sceneManager, *renderTargetManager);
	sceneManager->registerObservers(); //Needs to be called after Renderer creation

	static_cast<RenderWindow*>(renderWindow.get())->registerObserver(windowListener);
	inputManager->registerObserver(keyboardListener);

	resourceFilesWatcher.run(initializationParameters);
	
	return true;
}

bool Root::initResources(const IPlatformFactory *platformFactory, const std::string &resourcesFilePath)
{
	resourcesFile = new TiXmlDocument(resourcesFilePath);
	if (!resourcesFile->LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + resourcesFilePath + " does not exist or is malformed.");
		return false;
	}

	const TiXmlElement *resourcesElement = resourcesFile->FirstChildElement();
	if (!resourcesElement)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + resourcesFilePath + " does not contain <resources> element.");
		return false;
	}

	textureManager = platformFactory->getTextureManager(*resourcesFile);
	gpuBufferManager = platformFactory->getGpuBufferManager();
	gpuProgramManager = platformFactory->getGpuProgramManager();
	inputDescriptionManager = platformFactory->getInputDescriptionManager();
	gpuProgramStageManager = platformFactory->getGpuProgramStageManager(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.shadersFolder);

	imageManager = new ImageManager(new ImageBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.imagesFolder));
	meshManager = new MeshManager(new MeshBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.meshesFolder));
	materialManager = new MaterialManager(new MaterialBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.materialsFolder));

	imageManager->initializeFallbackObject();
	gpuProgramManager->initializeFallbackObject();
	textureManager->initializeFallbackObject();
	materialManager->initializeFallbackObject();
	meshManager->initializeFallbackObject();

	return true;
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