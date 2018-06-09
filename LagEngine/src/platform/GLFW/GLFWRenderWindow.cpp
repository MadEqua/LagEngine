#include "GLFWRenderWindow.h"
#include "LogManager.h"
#include "InitializationParameters.h"

#include "GLFW/glfw3.h"

using namespace Lag;

//GLOBAL REFERENCE FOR CALLBACKS
GLFWRenderWindow *glfwRenderWindow;

void Lag::windowCloseCallback(GLFWwindow *window) {
    glfwRenderWindow->onCloseNotify(*glfwRenderWindow);
}

void Lag::windowSizeCallback(GLFWwindow *window, int width, int height) {
    if(width > 0 && height > 0)
        glfwRenderWindow->resize(static_cast<uint32>(width), static_cast<uint32>(height));
}

void Lag::windowPosCallback(GLFWwindow *window, int xpos, int ypos) {
    glfwRenderWindow->onMoveNotify(*glfwRenderWindow, xpos, ypos);
}

void Lag::windowFocusCallback(GLFWwindow *window, int focused) {
    glfwRenderWindow->onFocusChangeNotify(*glfwRenderWindow, focused == GLFW_FOCUSED);
}

void Lag::errorCallback(int error, const char *description) {
    LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GLFWRenderWindow",
                                  "Error " + std::to_string(error) + ": " + description);
}


GLFWRenderWindow::GLFWRenderWindow(const InitializationParameters &parameters) :
        RenderWindow(parameters) {
    glfwRenderWindow = this;
}

bool GLFWRenderWindow::loadImplementation() {
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GLFWRenderWindow", "Cannot initialize GLFW.");
        return false;
    }
    else {
        glfwWindowHint(GLFW_SAMPLES, initializationParameters.MSAAsamples);
        glfwWindowHint(GLFW_SRGB_CAPABLE, initializationParameters.sRGB ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef ENABLE_DEBUG_MACRO
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

        window = glfwCreateWindow(initializationParameters.width, initializationParameters.height,
                                  initializationParameters.title.c_str(), nullptr, nullptr);

        if (window == nullptr) {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GLFWRenderWindow", "Cannot build GLFW window.");
            return false;
        }

        glfwSetWindowCloseCallback(window, windowCloseCallback);
        glfwSetWindowSizeCallback(window, windowSizeCallback);
        glfwSetWindowPosCallback(window, windowPosCallback);
        glfwSetWindowFocusCallback(window, windowFocusCallback);


        glfwMakeContextCurrent(window);
        if (!glfwGetCurrentContext()) {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                          "GLFWRenderWindow", "Cannot build OpenGL context.");
            return false;
        }

        glfwSwapInterval(initializationParameters.vsync ? GLFW_TRUE : GLFW_FALSE);
        setVirtualCursor(false);

        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      "GLFWRenderWindow", "Initialized successfully.");

        return true;
    }
}

void GLFWRenderWindow::unloadImplementation() {
    glfwSetWindowCloseCallback(window, nullptr);
    glfwSetWindowSizeCallback(window, nullptr);
    glfwSetWindowPosCallback(window, nullptr);
    glfwSetWindowFocusCallback(window, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                  "GLFWRenderWindow", "Destroyed successfully.");
}

void GLFWRenderWindow::processEvents() {
    glfwPollEvents();
}

void GLFWRenderWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLFWRenderWindow::setVirtualCursor(bool value) {
    virtualCursor = value;
    int v = value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(window, GLFW_CURSOR, v);
}

bool GLFWRenderWindow::isVirtualCursorEnabled() {
    return virtualCursor;
}