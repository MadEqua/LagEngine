#include "Root.h"

#include "InitializationParameters.h"
#include "io/log/LogManager.h"
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

#include "io/tinyxml/tinyxml.h"

using namespace Lag;

Root::Root() :
	renderWindow(nullptr),
	inputManager(nullptr),
	renderer(nullptr),
	sceneManager(nullptr),
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
	inputDescriptionManager(nullptr)
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
	if (gpuProgramStageManager != nullptr)
	{
		delete gpuProgramStageManager;
		gpuProgramStageManager = nullptr;
	}
	if (gpuProgramManager != nullptr)
	{
		delete gpuProgramManager;
		gpuProgramManager = nullptr;
	}
	if (materialManager != nullptr)
	{
		delete materialManager;
		materialManager = nullptr;
	}
	if (meshManager != nullptr)
	{
		delete meshManager;
		meshManager = nullptr;
	}
	if (imageManager != nullptr)
	{
		delete imageManager;
		imageManager = nullptr;
	}
	if (textureManager != nullptr)
	{
		delete textureManager;
		textureManager = nullptr;
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
	if (sceneManager != nullptr)
	{
		delete sceneManager;
		sceneManager = nullptr;
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
	renderWindow = new GLFWRenderWindow(parameters);
	if (!renderWindow->initialize())
		return false;

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
	{
		graphicsAPI = new GL4GraphicsAPI();
		textureManager = new GL4TextureManager();
		gpuBufferManager = new GL4GpuBufferManager();
		gpuProgramManager = new GL4GpuProgramManager();
		inputDescriptionManager = new GL4InputDescriptionManager();
	}

	if (!initResources(parameters.resourcesFolder + '/' + parameters.resourcesFile))
		return false;

	renderer = new Renderer(*graphicsAPI, *sceneManager);
	renderer->addRenderWindow(*renderWindow);

	windowListener = new WindowListener();
	keyboardListener = new KeyboardListener();
	renderWindow->registerObserver(*windowListener);
	inputManager->registerObserver(*keyboardListener);

	return true;
}

bool Root::initResources(const std::string &resourcesFilePath)
{
	TiXmlDocument doc(resourcesFilePath);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + resourcesFilePath + " does not exist or is malformed.");
		return false;
	}

	const TiXmlElement *resourcesElement = doc.FirstChildElement();
	if (!resourcesElement)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + resourcesFilePath + " does not contain <resources> element.");
		return false;
	}

	imageManager = new ImageManager(initializationParameters.resourcesFolder + '/' + initializationParameters.imagesFolder);

	//Initialize all ResourceManagers here
	imageManager->parseResourceFile(*resourcesElement);
	textureManager->parseResourceFile(*resourcesElement);

	meshManager = new MeshManager(initializationParameters.resourcesFolder + '/' + initializationParameters.meshesFolder);
	meshManager->parseResourceFile(*resourcesElement);

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
		gpuProgramStageManager = new GL4GpuProgramStageManager(initializationParameters.resourcesFolder + '/' + initializationParameters.shadersFolder);

	gpuProgramStageManager->parseResourceFile(*resourcesElement);

	materialManager = new MaterialManager(initializationParameters.resourcesFolder + '/' + initializationParameters.materialsFolder);
	materialManager->parseResourceFile(*resourcesElement);

	sceneManager = new SceneManager(initializationParameters.resourcesFolder + '/' + initializationParameters.scenesFolder);
	sceneManager->parseResourceFile(*resourcesElement);
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

void Root::unloadAllResourcesExceptScenes()
{
	if (gpuProgramStageManager != nullptr)
		gpuProgramStageManager->unloadAll();
	if (gpuProgramManager != nullptr)
		gpuProgramManager->unloadAll();
	if (materialManager != nullptr)
		materialManager->unloadAll();
	if (meshManager != nullptr)
		meshManager->unloadAll();
	if (imageManager != nullptr)
		imageManager->unloadAll();
	if (textureManager != nullptr)
		textureManager->unloadAll();
	if (gpuBufferManager != nullptr)
		gpuBufferManager->unloadAll();
	if (inputDescriptionManager != nullptr)
		inputDescriptionManager->unloadAll();

	//sceneManager->unloadAll();
}

void Root::KeyboardListener::onKeyPress(int key, int modifier)
{
	switch (key)
	{
	case KEY_LEFT_CONTROL:
	{
		RenderWindow &rw = Root::getInstance().getRenderWindow();
		rw.setVirtualCursor(!rw.isVirtualCursorEnabled());
	} break;
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