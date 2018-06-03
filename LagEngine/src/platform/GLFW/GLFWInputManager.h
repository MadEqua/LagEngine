#pragma once

#include "InputManager.h"

struct GLFWwindow;

namespace Lag {
    class GLFWRenderWindow;

    //Callbacks for GLFW
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);


    class GLFWInputManager : public InputManager {
        friend void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        friend void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
        friend void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    public:
        explicit GLFWInputManager(const GLFWRenderWindow &renderWndow);
        ~GLFWInputManager() override;

    private:
        GLFWwindow *window;
        int cursorX, cursorY;
    };
}