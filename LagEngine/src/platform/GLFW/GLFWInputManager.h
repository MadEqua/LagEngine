#pragma once

#include "../../io/InputManager.h"

struct GLFWwindow;

//Callbacks for GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

namespace Lag
{
	class GLFWRenderWindow;

	class GLFWInputManager : public InputManager
	{
	public:
		GLFWInputManager(GLFWRenderWindow *renderWndow);
		virtual ~GLFWInputManager();

	private:
		GLFWwindow *window;
		int cursorX, cursorY;

		//To access the private cursor position functions without exposing them
		friend void ::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		friend void ::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		inline void updateCursorCoords(int x, int y) {cursorX = x; cursorY = y; }
		inline int getCursorX() { return cursorX; }
		inline int getCursorY() { return cursorY; }
	};
}