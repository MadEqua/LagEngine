#pragma once

#include "../../renderer/RenderWindow.h"

struct GLFWwindow;

namespace Lag
{
	void windowCloseCallback(GLFWwindow* window);
	void windowSizeCallback(GLFWwindow* window, int width, int height);
	void windowPosCallback(GLFWwindow* window, int xpos, int ypos);
	void windowFocusCallback(GLFWwindow* window, int focused);


	class GLFWRenderWindow : public RenderWindow
	{
		friend void windowCloseCallback(GLFWwindow* window);
		friend void windowSizeCallback(GLFWwindow* window, int width, int height);
		friend void windowPosCallback(GLFWwindow* window, int xpos, int ypos);
		friend void windowFocusCallback(GLFWwindow* window, int focused);

	public:
		GLFWRenderWindow(const InitializationParameters &parameters);
		virtual ~GLFWRenderWindow();

		virtual bool initialize() override;
		virtual void destroy() override;

		virtual void processEvents() override;
		virtual void swapBuffers() override;

		inline GLFWwindow* getGLFWwindow() const { return window; }

	private:
		GLFWwindow *window;
	};
}