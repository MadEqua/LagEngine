#pragma once

#include "RenderWindow.h"

struct GLFWwindow;

namespace Lag {
    void windowCloseCallback(GLFWwindow *window);
    void windowSizeCallback(GLFWwindow *window, int width, int height);
    void windowPosCallback(GLFWwindow *window, int xpos, int ypos);
    void windowFocusCallback(GLFWwindow *window, int focused);
    void errorCallback(int error, const char *description);


    class GLFWRenderWindow : public RenderWindow {
        friend void windowCloseCallback(GLFWwindow *window);
        friend void windowSizeCallback(GLFWwindow *window, int width, int height);
        friend void windowPosCallback(GLFWwindow *window, int xpos, int ypos);
        friend void windowFocusCallback(GLFWwindow *window, int focused);
        friend void errorCallback(int error, const char *description);

    public:
        explicit GLFWRenderWindow(const InitializationParameters &parameters);

        bool loadImplementation() override;
        void unloadImplementation() override;

        void processEvents() override;
        void swapBuffers() override;

        void setVirtualCursor(bool value) override;
        bool isVirtualCursorEnabled() override;

        inline GLFWwindow *getGLFWwindow() const { return window; }

    private:
        GLFWwindow *window;
        bool virtualCursor;
    };
}