#include "Root.h"

#include <thread>
#include <chrono>
#include <iostream>

#include "platform/GLFW/GLFWRenderWindow.h"
#include "platform/GLFW/GLFWInputManager.h"

using namespace Lag;

Root::Root() :
	renderWindow(nullptr),
	inputManager(nullptr),
	logManager(nullptr),
	windowListener(nullptr)
{
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
	if (logManager != nullptr)
		delete logManager;
	if (windowListener != nullptr)
		delete windowListener;
}

bool Root::initializeLag(const RenderWindowParameters &parameters)
{
	//in case of reinitialization
	destroy();

	logManager = new LogManager();
	windowListener = new WindowListener();

	minFrameTime = parameters.maxFPS > 0 ? 1.0f / parameters.maxFPS : 0.0f;

	//TODO auto detect platform?
	if (parameters.platform == GLFW)
	{
		renderWindow = new GLFWRenderWindow(parameters);
		if (!renderWindow->initialize())
			return false;

		inputManager = new GLFWInputManager(static_cast<GLFWRenderWindow*>(renderWindow));
	}
	else if (parameters.platform == ANDROID)
	{
		//TODO
	}

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