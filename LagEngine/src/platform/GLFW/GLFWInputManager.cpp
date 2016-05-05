#include "GLFWInputManager.h"

#include "../../core/log/LogManager.h"
#include "../../Root.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWInputManager *inputManager;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		inputManager->onKeyPressNotify(key, mods);
	case GLFW_RELEASE:
		inputManager->onKeyReleaseNotify(key, mods);
	case GLFW_REPEAT:
		inputManager->onKeyRepeatNotify(key, mods);
	default:
		break;
	}
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) 
{
	int x = static_cast<int>(xpos);
	int y = static_cast<int>(ypos);
	inputManager->updateCursorCoords(x, y);
	inputManager->onCursorMoveNotify(x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
	switch (action)
	{
	case GLFW_PRESS:
		inputManager->onButtonPressedNotify(inputManager->getCursorX(), inputManager->getCursorY(), button, mods);
	case GLFW_RELEASE:
		inputManager->onButtonReleasedNotify(inputManager->getCursorX(), inputManager->getCursorY(), button, mods);
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

	Root::getInstance().getLogManager().log(FILE, NORMAL, INFO, "GLFWInputManager", "Initialized successfully.");
}

GLFWInputManager::~GLFWInputManager()
{
	glfwSetKeyCallback(window, 0);
	glfwSetCursorPosCallback(window, 0);
	glfwSetMouseButtonCallback(window, 0);

	Root::getInstance().getLogManager().log(FILE, NORMAL, INFO, "GLFWInputManager", "Destroyed successfully.");
}