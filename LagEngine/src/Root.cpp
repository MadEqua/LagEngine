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
		delete gpuProgramStageManager;
	if (gpuProgramManager != nullptr)
		delete gpuProgramManager;
	if (materialManager != nullptr)
		delete materialManager;
	if (meshManager != nullptr)
		delete meshManager;
	if (imageManager != nullptr)
		delete imageManager;
	if (textureManager != nullptr)
		delete textureManager;

	if (gpuBufferManager != nullptr)
		delete gpuBufferManager;

	if (graphicsAPI != nullptr)
		delete graphicsAPI;

	if (renderer != nullptr)
		delete renderer;
	if (sceneManager != nullptr)
		delete sceneManager;

	if (inputDescriptionManager != nullptr)
		delete inputDescriptionManager;

	if (windowListener != nullptr)
		delete windowListener;
	if (keyboardListener != nullptr)
		delete keyboardListener;

	if (inputManager != nullptr)
		delete inputManager;
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

	sceneManager = new SceneManager();

	//TODO Use a factory to initialize render window and graphics api?
	renderWindow = new GLFWRenderWindow(parameters);
	if (!renderWindow->initialize())
		return false;

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
		graphicsAPI = new GL4GraphicsAPI();

	renderer = new Renderer(*graphicsAPI, *sceneManager);

	renderer->addRenderWindow(*renderWindow);

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));
	imageManager = new ImageManager(initializationParameters.resourcesFolder + '/' + initializationParameters.imagesFolder);

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
	{
		textureManager = new GL4TextureManager();
		gpuBufferManager = new GL4GpuBufferManager();
		gpuProgramManager = new GL4GpuProgramManager();
		inputDescriptionManager = new GL4InputDescriptionManager();
	}

	if (!initResources(parameters.resourcesFolder + '/' + parameters.resourcesFile))
		return false;

	windowListener = new WindowListener();
	keyboardListener = new KeyboardListener();
	renderWindow->registerObserver(*windowListener);
	inputManager->registerObserver(*keyboardListener);

	//Load the declared resources
	imageManager->loadAll();
	textureManager->loadAll();
	meshManager->loadAll();
	gpuProgramStageManager->loadAll();
	materialManager->loadAll();
	gpuProgramManager->loadAll();

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

	const TiXmlElement *resourcesElement = nullptr;
	for (const TiXmlElement* child = doc.FirstChildElement();
		child != 0;
		child = child->NextSiblingElement())
	{
		if (child->ValueStr() == "resources")
		{
			resourcesElement = child;
			break;
		}
	}

	if (!resourcesElement)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL, "Root",
			"Resources file: " + resourcesFilePath + " does not contain <resources> element.");
		return false;
	}

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