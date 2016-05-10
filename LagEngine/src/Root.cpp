#include "Root.h"

#include <thread>
#include <chrono>

#include "renderer/RenderWindow.h"
#include "renderer/RenderWindowParameters.h"
#include "io/InputManager.h"
#include "io/ini/IniManager.h"
#include "io/log/LogManager.h"
#include "platform/GLFW/GLFWRenderWindow.h"
#include "platform/GLFW/GLFWInputManager.h"
#include "renderer/Renderer.h"
#include "renderer/SceneGraph.h"

using namespace Lag;

Root::Root() :
	renderWindow(nullptr),
	inputManager(nullptr),
	renderer(nullptr),
	sceneGraph(nullptr),
	windowListener(nullptr)
{
	//Initialize other singletons
	LogManager::getInstance();
	IniManager::getInstance();
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
	if (sceneGraph != nullptr)
		delete sceneGraph;
	if (windowListener != nullptr)
		delete windowListener;
}

bool Root::initializeLag(const RenderWindowParameters &parameters)
{
	return internalInit(parameters);
}

bool Root::initializeLag(const std::string &iniFile)
{
	RenderWindowParameters parameters(iniFile);
	return internalInit(parameters);
}

bool Root::internalInit(const RenderWindowParameters &parameters)
{
	//in case of reinitialization
	destroy();

	windowListener = new WindowListener();

	minFrameTime = parameters.maxFPS > 0 ? 1.0f / parameters.maxFPS : 0.0f;

	sceneGraph = new SceneGraph();

	//TODO auto detect platform?
	renderWindow = new GLFWRenderWindow(parameters);
	if (!renderWindow->initialize())
		return false;

	renderer = new Renderer();
	if (!renderer->initialize(parameters.gpuInterface))
		return false;

	inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));

	renderWindow->registerObserver(*windowListener);

	return true;
}

void Root::startRenderingLoop()
{
	shouldLoop = true;
	while (shouldLoop)
	{
		frameTimer.start();

		renderWindow->processEvents();
		renderOneFrame();
		renderWindow->swapBuffers();

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
	//TODO
	//update
	//render
	/*int i = 0;
	while (i++ < 1000000)
		;*/

	//logManager->log(CONSOLE, NORMAL, DEBUG, "FRAME", "FRAME");
}