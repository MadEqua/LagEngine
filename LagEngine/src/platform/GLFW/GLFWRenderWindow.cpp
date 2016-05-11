#include "GLFWRenderWindow.h"
#include "GLFW/glfw3.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWRenderWindow *glfwRenderWindow;

void Lag::windowCloseCallback(GLFWwindow * window)
{
	glfwRenderWindow->onCloseNotify();
}

void Lag::windowSizeCallback(GLFWwindow * window, int width, int height)
{
	glfwRenderWindow->onResizeNotify(width, height);
}

void Lag::windowPosCallback(GLFWwindow * window, int xpos, int ypos)
{
	glfwRenderWindow->onMoveNotify(xpos, ypos);
}

void Lag::windowFocusCallback(GLFWwindow * window, int focused)
{
	glfwRenderWindow->onFocusChangeNotify(focused == GLFW_FOCUSED ? true : false);
}


GLFWRenderWindow::GLFWRenderWindow(const RenderWindowParameters &parameters) :
	RenderWindow(parameters)
{
	glfwRenderWindow = this;
}

GLFWRenderWindow::~GLFWRenderWindow()
{
	glfwSetWindowCloseCallback(window, 0);
	glfwSetWindowSizeCallback(window, 0);
	glfwSetWindowPosCallback(window, 0);
	glfwSetWindowFocusCallback(window, 0);
	destroy();

	LogManager::getInstance().log(FILE, NORMAL, INFO, "GLFWRenderWindow", "Destroyed successfully.");
}

bool GLFWRenderWindow::initialize()
{
	if (!glfwInit())
	{
		LogManager::getInstance().log(FILE, NORMAL, ERROR, "glfwInit()", "Cannot initialize GLFW.");
		return false;
	}
	else
	{
		glfwWindowHint(GLFW_SAMPLES, parameters.MSSAsamples);
		glfwWindowHint(GLFW_SRGB_CAPABLE, parameters.sRGB ? GL_TRUE : GL_FALSE);
		glfwSwapInterval(parameters.vsync ? 1 : 0);

		window = glfwCreateWindow(parameters.width, parameters.height, parameters.title.c_str(), NULL, NULL);
		if (window == 0)
		{
			LogManager::getInstance().log(FILE, NORMAL, ERROR, "glfwCreateWindow()", "Cannot create GLFW window.");
			return false;
		}

		glfwSetWindowCloseCallback(window, windowCloseCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		glfwSetWindowPosCallback(window, windowPosCallback);
		glfwSetWindowFocusCallback(window, windowFocusCallback);

		LogManager::getInstance().log(FILE, NORMAL, INFO, "GLFWRenderWindow", "Initialized successfully.");

		return true;
	}
}

void GLFWRenderWindow::destroy()
{
	glfwTerminate();
}

void GLFWRenderWindow::processEvents()
{
	glfwPollEvents();
}

void GLFWRenderWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}
