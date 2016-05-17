#pragma once

#include <string>

#include "core/Timer.h"
#include "InitializationParameters.h"
#include "renderer/IWindowListener.h"
#include "core/SingletonPattern.h"
#include "core/ObserverPattern.h"

namespace Lag
{
	class WindowListener;
	class RenderWindow;
	class InputManager;
	class Renderer;
	class SceneManager;
	class IFrameListener;

	class GpuProgramStageManager;
	class GpuProgramManager;
	class MaterialManager;
	class MeshManager;

	class Root
	{
		GENERATE_SINGLETON(Root)

		GENERATE_OBSERVER_STORAGE(IFrameListener)
		DECLARE_NOTIFY_METHOD(onFrameStart, ARGS(float timePassed))
		DECLARE_NOTIFY_METHOD(onFrameRenderingQueued, ARGS(float timePassed))
		DECLARE_NOTIFY_METHOD(onFrameEnd, ARGS(float timePassed))


	public:
		bool initializeLag(const InitializationParameters &parameters);
		bool initializeLag(const std::string &iniFile);
		
		inline RenderWindow& getRenderWindow() const { return *renderWindow; }
		inline InputManager& getInputManager() const { return *inputManager; }
		inline Renderer& getRenderer() const { return *renderer; }
		inline SceneManager& getSceneManager() const { return *sceneManager; }
		inline const InitializationParameters& getInitializationParameters() const { return initializationParameters; }

		inline GpuProgramStageManager& getGpuProgramStageManager() const { return *gpuProgramStageManager; }
		inline GpuProgramManager& getGpuProgramManager() const { return *gpuProgramManager; }
		inline MaterialManager& getMaterialManager() const { return *materialManager; }
		inline MeshManager& getMeshManager() const { return *meshManager; }

		void startRenderingLoop();
		void stopRenderingLoop();

	private:
		RenderWindow *renderWindow;
		InputManager *inputManager;
		Renderer *renderer;
		SceneManager *sceneManager;

		GpuProgramStageManager *gpuProgramStageManager;
		GpuProgramManager *gpuProgramManager;
		MaterialManager *materialManager;
		MeshManager *meshManager;


		WindowListener *windowListener;

		InitializationParameters initializationParameters;


		//Frame Timing
		Timer frameTimer;
		float minFrameTime; //min frame time to respect the max FPS

		bool shouldLoop;

		void renderOneFrame();
		void destroy();
		bool internalInit(const InitializationParameters &parameters);
		bool initResources(const std::string &resourcesFilePath);
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