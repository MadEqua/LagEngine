#pragma once

#include "../../rendering/RenderWindow.h"

struct GLFWwindow;

void windowCloseCallback(GLFWwindow* window);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowPosCallback(GLFWwindow* window, int xpos, int ypos);
void windowFocusCallback(GLFWwindow* window, int focused);


namespace Lag
{
	class GLFWRenderWindow : public RenderWindow
	{
	public:
		GLFWRenderWindow(const RenderWindowParameters &parameters);
		virtual ~GLFWRenderWindow();

		virtual bool initialize();
		virtual void destroy();

		virtual void processEvents();
		virtual void swapBuffers();

		inline GLFWwindow* getGLFWwindow() { return window; }

	private:
		GLFWwindow *window;
	};
}