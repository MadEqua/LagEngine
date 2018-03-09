#include "Root.h"

#include "io/tinyxml/tinyxml.h"

#include "InitializationParameters.h"
#include "io/log/LogManager.h"

#include "platform/GLFW/GLFWRenderTargetBuilder.h"
#include "graphicsAPIs/gl4/GL4RenderTargetBuilder.h"

#include "platform/GLFW/GLFWRenderWindow.h"
#include "platform/GLFW/GLFWInputManager.h"
#include "renderer/Renderer.h"
#include "scene/SceneManager.h"

#include "graphicsAPIs/gl4/GL4GpuProgramStageManager.h"
#include "graphicsAPIs/gl4/GL4GpuProgramManager.h"

#include "resources/MaterialManager.h"
#include "resources/MeshManager.h"
#include "resources/ImageManager.h"
#include "graphicsAPIs/gl4/GL4TextureManager.h"
#include "graphicsAPIs/gl4/GL4GpuBufferManager.h"

#include "graphicsAPIs/gl4/GL4GraphicsAPI.h"
#include "graphicsAPIs/gl4/GL4InputDescriptionManager.h"

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
	windowListener(nullptr),
	keyboardListener(nullptr),
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

	if (windowListener != nullptr)
	{
		delete windowListener;
		windowListener = nullptr;
	}
	if (keyboardListener != nullptr)
	{
		delete keyboardListener;
		keyboardListener = nullptr;
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

bool Root::initializeLag(const InitializationParameters &parameters)
{
	return internalInit(parameters);
}

bool Root::initializeLag(const std::string &iniFile)
{
	InitializationParameters parameters(iniFile);
	return internalInit(parameters);
}

bool Root::internalInit(const InitializationParameters &parameters)
{
	initializationParameters = parameters;
	
	//in case of reinitialization
	destroy();

	//TODO Use a factory to initialize render window and graphics api?
	renderTargetManager = new RenderTargetManager(new GLFWRenderTargetBuilder(), new GL4RenderTargetBuilder());
	renderWindow = renderTargetManager->getRenderWindow(initializationParameters);
	if (!renderWindow.isValid())
		return false;

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow.get()));

	if (!initResources(parameters.resourcesFolder + '/' + parameters.resourcesFile))
		return false;

	sceneManager = new SceneManager();
	renderer = new Renderer(*graphicsAPI, *sceneManager, *renderTargetManager);
	sceneManager->registerObservers(); //Needs to be called after Renderer creation

	windowListener = new WindowListener();
	keyboardListener = new KeyboardListener(static_cast<GLFWRenderWindow*>(renderWindow.get()));
	static_cast<RenderWindow*>(renderWindow.get())->registerObserver(*windowListener);
	inputManager->registerObserver(*keyboardListener);

	return true;
}

bool Root::initResources(const std::string &resourcesFilePath)
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

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
	{
		graphicsAPI = new GL4GraphicsAPI();
		if (!graphicsAPI->initialize())
			return false;

		textureManager = new GL4TextureManager(new GL4TextureBuilder(*resourcesFile));
		gpuBufferManager = new GL4GpuBufferManager();
		gpuProgramManager = new GL4GpuProgramManager();
		inputDescriptionManager = new GL4InputDescriptionManager();
		gpuProgramStageManager = new GL4GpuProgramStageManager(new GL4GpuProgramStageBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.shadersFolder));
	}

	imageManager = new ImageManager(new ImageBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.imagesFolder));
	meshManager = new MeshManager(new MeshBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.meshesFolder));
	materialManager = new MaterialManager(new MaterialBuilder(*resourcesFile, initializationParameters.resourcesFolder + '/' + initializationParameters.materialsFolder));

	renderTargetManager->initialize();
	gpuBufferManager->initialize();
	inputDescriptionManager->initialize();
	imageManager->initialize();
	gpuProgramStageManager->initialize();
	gpuProgramManager->initialize();
	textureManager->initialize();
	materialManager->initialize();
	meshManager->initialize();

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

//This clear order is important, it's the dependency order.
//TODO: find a better solution
void Root::clearUnusedResources()
{
	renderTargetManager->clearUnused();
	
	meshManager->clearUnused();
	inputDescriptionManager->clearUnused();
	gpuBufferManager->clearUnused();

	materialManager->clearUnused();
	gpuProgramManager->clearUnused();
	textureManager->clearUnused();
	gpuProgramStageManager->clearUnused();
	imageManager->clearUnused();
}

Root::KeyboardListener::KeyboardListener(RenderWindow *renderWindow) :
	renderWindow(renderWindow)
{
}

void Root::KeyboardListener::onKeyPress(int key, int modifier)
{
	switch (key)
	{
	case KEY_LEFT_CONTROL:
		renderWindow->setVirtualCursor(!renderWindow->isVirtualCursorEnabled());
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