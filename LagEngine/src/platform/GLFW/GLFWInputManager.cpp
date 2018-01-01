#include "GLFWInputManager.h"

#include "GLFWRenderWindow.h"
#include "../../io/log/LogManager.h"

#include "GLFW/glfw3.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWInputManager *inputManager;

void Lag::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		inputManager->onKeyPressNotify(key, mods);
		break;
	case GLFW_RELEASE:
		inputManager->onKeyReleaseNotify(key, mods);
		break;
	case GLFW_REPEAT:
		inputManager->onKeyRepeatNotify(key, mods);
		break;
	default:
		break;
	}
}

void Lag::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	int x = static_cast<int>(xpos);
	int y = static_cast<int>(ypos);
	inputManager->cursorX = x;
	inputManager->cursorY = y;
	inputManager->onCursorMoveNotify(x, y);
}

void Lag::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		inputManager->onButtonPressedNotify(inputManager->cursorX, inputManager->cursorY, button, mods);
		break;
	case GLFW_RELEASE:
		inputManager->onButtonReleasedNotify(inputManager->cursorX, inputManager->cursorY, button, mods);
		break;
	default:
		break;
	}
}

GLFWInputManager::GLFWInputManager(GLFWRenderWindow *renderWindow) : window(renderWindow->getGLFWwindow())
{
	inputManager = this;
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GLFWInputManager", "Initialized successfully.");
}

GLFWInputManager::~GLFWInputManager()
{
	glfwSetKeyCallback(window, 0);
	glfwSetCursorPosCallback(window, 0);
	glfwSetMouseButtonCallback(window, 0);

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GLFWInputManager", "Destroyed successfully.");
}