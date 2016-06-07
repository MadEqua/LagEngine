#include "GLFWRenderWindow.h"
#include "GLFW/glfw3.h"
#include "../../io/log/LogManager.h"
#include "../../InitializationParameters.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWRenderWindow *glfwRenderWindow;

void Lag::windowCloseCallback(GLFWwindow * window)
{
	glfwRenderWindow->onCloseNotify(*glfwRenderWindow);
}

void Lag::windowSizeCallback(GLFWwindow * window, int width, int height)
{
	glfwRenderWindow->resize(width, height);
}

void Lag::windowPosCallback(GLFWwindow * window, int xpos, int ypos)
{
	glfwRenderWindow->onMoveNotify(*glfwRenderWindow, xpos, ypos);
}

void Lag::windowFocusCallback(GLFWwindow * window, int focused)
{
	glfwRenderWindow->onFocusChangeNotify(*glfwRenderWindow, focused == GLFW_FOCUSED ? true : false);
}


GLFWRenderWindow::GLFWRenderWindow(const InitializationParameters &parameters) :
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

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GLFWRenderWindow", "Destroyed successfully.");
}

bool GLFWRenderWindow::initialize()
{
	if (!glfwInit())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GLFWRenderWindow", "Cannot initialize GLFW.");
		return false;
	}
	else
	{
		glfwWindowHint(GLFW_SAMPLES, initializationParameters.MSSAsamples);
		glfwWindowHint(GLFW_SRGB_CAPABLE, initializationParameters.sRGB ? 1 : 0);
		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
		glfwWindowHint(GLFW_AUX_BUFFERS, 0);

		glfwSwapInterval(initializationParameters.vsync ? 1 : 0);

		window = glfwCreateWindow(initializationParameters.width, 
			initializationParameters.height, 
			initializationParameters.title.c_str(),
			NULL, NULL);

		if (window == 0)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				"GLFWRenderWindow", "Cannot create GLFW window.");
			return false;
		}

		glfwSetWindowCloseCallback(window, windowCloseCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		glfwSetWindowPosCallback(window, windowPosCallback);
		glfwSetWindowFocusCallback(window, windowFocusCallback);


		glfwMakeContextCurrent(window);
		if (!glfwGetCurrentContext()) 
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				"GLFWRenderWindow", "Cannot create OpenGL context.");
			return false;
		}

		setVirtualCursor(false);

		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GLFWRenderWindow", "Initialized successfully.");

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

void GLFWRenderWindow::setVirtualCursor(bool value)
{
	virtualCursor = value;
	int v = value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
	glfwSetInputMode(window, GLFW_CURSOR, v);
}

bool GLFWRenderWindow::isVirtualCursorEnabled()
{
	return virtualCursor;
}