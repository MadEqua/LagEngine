#include "Root.h"

#include <thread>
#include <chrono>

#include "renderer/RenderWindow.h"
#include "InitializationParameters.h"
#include "io/InputManager.h"
#include "io/log/LogManager.h"
#include "platform/GLFW/GLFWRenderWindow.h"
#include "platform/GLFW/GLFWInputManager.h"
#include "resources/GpuProgramStageManager.h"
#include "renderer/Renderer.h"
#include "scene/SceneManager.h"
#include "IFrameListener.h"

#include "io/tinyxml/tinyxml.h"

using namespace Lag;

DEFINE_NOTIFY_METHOD(Root, onFrameStart, IFrameListener, ARGS(float timePassed), ARGS(timePassed))
DEFINE_NOTIFY_METHOD(Root, onFrameRenderingQueued, IFrameListener, ARGS(float timePassed), ARGS(timePassed))
DEFINE_NOTIFY_METHOD(Root, onFrameEnd, IFrameListener, ARGS(float timePassed), ARGS(timePassed))

Root::Root() :
	renderWindow(nullptr),
	inputManager(nullptr),
	renderer(nullptr),
	sceneManager(nullptr),
	gpuProgramStageManager(nullptr),
	windowListener(nullptr)
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
	if (renderWindow != nullptr)
		delete renderWindow;
	if (inputManager != nullptr)
		delete inputManager;
	if (renderer != nullptr)
		delete renderer;
	if (sceneManager != nullptr)
		delete sceneManager;
	if (gpuProgramStageManager != nullptr)
		delete gpuProgramStageManager;
	if (windowListener != nullptr)
		delete windowListener;
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

	windowListener = new WindowListener();

	minFrameTime = parameters.maxFPS > 0 ? 1.0f / parameters.maxFPS : 0.0f;

	sceneManager = new SceneManager();

	//TODO auto detect platform and refactor this code
	renderWindow = new GLFWRenderWindow(parameters);
	if (!renderWindow->initialize())
		return false;

	renderer = new Renderer(*sceneManager);
	if (!renderer->initialize(parameters.gpuInterfaceType))
		return false;

	renderer->addRenderTarget("renderWindow", *renderWindow);

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));

	if (!initResources(parameters.resourcesFile))
		return false;

	renderWindow->registerObserver(*windowListener);

	return true;
}

bool Root::initResources(const std::string &resourcesFilePath)
{
	TiXmlDocument doc(resourcesFilePath);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LogOutput::FILE, NORMAL, ERROR, "Root",
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
		LogManager::getInstance().log(LogOutput::FILE, NORMAL, ERROR, "Root",
			"Resources file: " + resourcesFilePath + " does not contain <resources> element.");
		return false;
	}

	//Init all ResourceManagers here
	gpuProgramStageManager = new GpuProgramStageManager(initializationParameters.gpuInterfaceType);
	gpuProgramStageManager->initalizeFromResourcesFile(*resourcesElement);

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