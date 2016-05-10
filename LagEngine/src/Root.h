#pragma once

#include <string>

#include "core/Timer.h"
#include "renderer/IWindowListener.h"
#include "core/SingletonPattern.h"

namespace Lag
{
	class WindowListener;
	class RenderWindow;
	class InputManager;
	class RenderWindowParameters;
	class Renderer;
	class SceneGraph;

	class Root
	{
		GENERATE_SINGLETON(Root)

	public:
		bool initializeLag(const RenderWindowParameters &parameters);
		bool initializeLag(const std::string &iniFile);
		
		inline InputManager& getInputManager() { return *inputManager; }
		inline RenderWindow& getRenderWindow() { return *renderWindow; }
		inline Renderer& getRenderer() { return *renderer; }
		inline SceneGraph& getSceneGraph() { return *sceneGraph; }

		void startRenderingLoop();
		void stopRenderingLoop();

	private:
		RenderWindow *renderWindow;
		InputManager *inputManager;
		Renderer *renderer;
		SceneGraph *sceneGraph;

		WindowListener *windowListener;

		//Frame Timing
		Timer frameTimer;
		float minFrameTime; //min frame time to respect the max FPS

		bool shouldLoop;

		void renderOneFrame();
		void destroy();
		bool internalInit(const RenderWindowParameters &parameters);
	};


	/*
	* Listen for window close and stop looping.
	*/
	class WindowListener : public IWindowListener
	{
	public:
		virtual void onMove(int x, int y) {}
		virtual void onResize(int width, int height) {}
		virtual void onFocusChange(bool focused) {}
		virtual void onClose()
		{
			Root::getInstance().stopRenderingLoop();
		}
	};
}