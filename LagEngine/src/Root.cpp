#include "Root.h"

#include <thread>
#include <chrono>

#include "renderer/RenderWindow.h"
#include "InitializationParameters.h"
#include "io/InputManager.h"
#include "io/log/LogManager.h"
#include "platform/GLFW/GLFWRenderWindow.h"
#include "platform/GLFW/GLFWInputManager.h"
#include "renderer/Renderer.h"
#include "scene/SceneManager.h"
#include "IFrameListener.h"

#include "graphicsAPIs/gl4/GL4GpuProgramStageManager.h"
#include "graphicsAPIs/gl4/GL4GpuProgramManager.h"

#include "resources/MaterialManager.h"
#include "resources/MeshManager.h"
#include "graphicsAPIs/gl4/GL4GpuBufferManager.h"

#include "graphicsAPIs/gl4/GL4GraphicsAPI.h"
#include "graphicsAPIs/gl4/GL4InputDescriptionManager.h"

#include "io/Keys.h"

#include "io/tinyxml/tinyxml.h"

using namespace Lag;

LAG_DEFINE_NOTIFY_METHOD(Root, onFrameStart, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
LAG_DEFINE_NOTIFY_METHOD(Root, onFrameRenderingQueued, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
LAG_DEFINE_NOTIFY_METHOD(Root, onFrameEnd, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))

Root::Root() :
	renderWindow(nullptr),
	inputManager(nullptr),
	renderer(nullptr),
	sceneManager(nullptr),
	gpuProgramStageManager(nullptr),
	gpuProgramManager(nullptr),
	materialManager(nullptr),
	meshManager(nullptr),
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
	if (renderWindow != nullptr)
		delete renderWindow;
}

bool Root::initializeLag(const InitializationParameters &parameters)
{
	initializationParameters = parameters;
	return internalInit(parameters);
}

bool Root::initializeLag(const std::string &iniFile)
{
	InitializationParameters parameters(iniFile);
	return internalInit(parameters);
}

bool Root::internalInit(const InitializationParameters &parameters)
{
	//in case of reinitialization
	destroy();

	minFrameTime = parameters.maxFPS > 0 ? 1.0f / parameters.maxFPS : 0.0f;

	sceneManager = new SceneManager();

	//TODO auto detect platform and refactor this code
	renderWindow = new GLFWRenderWindow(parameters);
	if (!renderWindow->initialize())
		return false;

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
		graphicsAPI = new GL4GraphicsAPI();

	renderer = new Renderer(*graphicsAPI, *sceneManager);

	renderer->addRenderTarget("renderWindow", *renderWindow);

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
	{
		gpuBufferManager = new GL4GpuBufferManager();
		gpuProgramManager = new GL4GpuProgramManager();
		inputDescriptionManager = new GL4InputDescriptionManager();
	}

	if (!initResources(parameters.resourcesFile))
		return false;

	windowListener = new WindowListener();
	keyboardListener = new KeyboardListener();
	renderWindow->registerObserver(*windowListener);
	inputManager->registerObserver(*keyboardListener);

	//Load the declared resources
	meshManager->loadAll();
	materialManager->loadAll();
	gpuProgramStageManager->loadAll();
	gpuProgramManager->loadAll();

	return true;
}

bool Root::initResources(const std::string &resourcesFilePath)
{
	TiXmlDocument doc(resourcesFilePath);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO, "Root",
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
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO, "Root",
			"Resources file: " + resourcesFilePath + " does not contain <resources> element.");
		return false;
	}

	//Initialize all ResourceManagers here
	meshManager = new MeshManager();
	meshManager->parseResourceFile(*resourcesElement);

	if (initializationParameters.graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
		gpuProgramStageManager = new GL4GpuProgramStageManager();

	gpuProgramStageManager->parseResourceFile(*resourcesElement);

	materialManager = new MaterialManager();
	materialManager->parseResourceFile(*resourcesElement);

	return true;
}

void Root::startRenderingLoop()
{
	shouldLoop = true;
	while (shouldLoop)
	{
		frameTimer.start();

		renderWindow->processEvents();

		onFrameStartNotify(0.0f); //TODO implement timing

		renderOneFrame();

		onFrameEndNotify(0.0f); //TODO implement timing

		float elapsed = frameTimer.getElapsedSeconds();
		//logManager->log(CONSOLE, NORMAL, DEBUG, "elapsed", std::to_string(elapsed));
		if (elapsed < minFrameTime)
		{
			std::chrono::duration<float> dur(minFrameTime - elapsed);
			std::this_thread::sleep_for(dur);
			//logManager->log(CONSOLE, NORMAL, DEBUG, "slept for", std::to_string(dur.count()));
		}
	}
}

void Root::stopRenderingLoop()
{
	shouldLoop = false;
}

void Root::renderOneFrame()
{
	renderer->renderAllRenderTargets();
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

void Root::WindowListener::onClose()
{
	Root::getInstance().stopRenderingLoop();
}