#include "GLFWRenderWindow.h"
#include "GLFW/glfw3.h"
#include "../../Root.h"
#include "../../core/log/LogManager.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWRenderWindow *glfwRenderWindow;

void windowCloseCallback(GLFWwindow * window)
{
	glfwRenderWindow->onCloseNotify();
}

void windowSizeCallback(GLFWwindow * window, int width, int height)
{
	glfwRenderWindow->onResizeNotify(width, height);
}

void windowPosCallback(GLFWwindow * window, int xpos, int ypos)
{
	glfwRenderWindow->onMoveNotify(xpos, ypos);
}

void windowFocusCallback(GLFWwindow * window, int focused)
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

	Root::getInstance().getLogManager().log(FILE, NORMAL, INFO, "GLFWRenderWindow", "Destroyed successfully.");
}

bool GLFWRenderWindow::initialize()
{
	if (!glfwInit())
	{
		Root::getInstance().getLogManager().log(FILE, NORMAL, ERROR, "glfwInit()", "Cannot initialize GLFW.");
		return false;
	}
	else
	{
		glfwWindowHint(GLFW_SAMPLES, parameters.msaaSamples);
		glfwWindowHint(GLFW_SRGB_CAPABLE, parameters.sRGB ? GL_TRUE : GL_FALSE);
		glfwSwapInterval(parameters.vsync ? 1 : 0);

		window = glfwCreateWindow(parameters.width, parameters.height, parameters.title.c_str(), NULL, NULL);
		if (window == 0)
		{
			Root::getInstance().getLogManager().log(FILE, NORMAL, ERROR, "glfwCreateWindow()", "Cannot create GLFW window.");
			return false;
		}

		glfwSetWindowCloseCallback(window, windowCloseCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		glfwSetWindowPosCallback(window, windowPosCallback);
		glfwSetWindowFocusCallback(window, windowFocusCallback);

		Root::getInstance().getLogManager().log(FILE, NORMAL, INFO, "GLFWRenderWindow", "Initialized successfully.");

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
