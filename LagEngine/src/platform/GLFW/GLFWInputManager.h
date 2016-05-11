#pragma once

#include "../../io/InputManager.h"

struct GLFWwindow;

namespace Lag
{
	class GLFWRenderWindow;
	
	//Callbacks for GLFW
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


	class GLFWInputManager : public InputManager
	{
		friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	public:
		GLFWInputManager(GLFWRenderWindow *renderWndow);
		virtual ~GLFWInputManager();

	private:
		GLFWwindow *window;
		int cursorX, cursorY;

		inline void updateCursorCoords(int x, int y) {cursorX = x; cursorY = y; }
		inline int getCursorX() { return cursorX; }
		inline int getCursorY() { return cursorY; }
	};
}