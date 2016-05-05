#pragma once

#include "rendering/RenderWindowParameters.h"
#include "rendering/RenderWindow.h"
#include "io/InputManager.h"
#include "core/log/LogManager.h"
#include "core/Timer.h"
#include "rendering/IWindowListener.h"

namespace Lag
{
	class WindowListener;

	class Root
	{
	public:
		bool initializeLag(const RenderWindowParameters &parameters);
		
		inline InputManager& getInputManager() { return *inputManager; }
		inline LogManager& getLogManager() { return *logManager; }
		inline RenderWindow& getRenderWindow() { return *renderWindow; }

		void startRenderingLoop();
		void stopRenderingLoop();


		////////////////////////////////
		//SINGLETON STUFF
		////////////////////////////////
		static Root& getInstance() {
			// Guaranteed to be lazy initialized and that it will be destroyed correctly
			static Root instance;
			return instance;
		}
	private:
		RenderWindow *renderWindow;
		InputManager *inputManager;
		LogManager *logManager;

		WindowListener *windowListener;

		//Frame Timing
		Timer frameTimer;
		float minFrameTime; //min frame time to respect the max FPS

		bool shouldLoop;

		void renderOneFrame();
		void destroy();

		////////////////////////////////
		//SINGLETON STUFF
		////////////////////////////////
		Root();
		~Root();

		Root(Root const& copy);            // Not Implemented
		Root& operator=(Root const& copy); // Not Implemented
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