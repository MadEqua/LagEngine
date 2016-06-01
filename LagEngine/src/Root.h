#pragma once

#include <string>

#include "core/Timer.h"
#include "InitializationParameters.h"

#include "renderer/IWindowListener.h"
#include "io/IKeyboardListener.h"

#include "core/SingletonPattern.h"
#include "core/ObserverPattern.h"

namespace Lag
{
	class RenderWindow;
	class InputManager;
	class Renderer;
	class SceneManager;
	class IFrameListener;

	class GpuProgramStageManager;
	class GpuProgramManager;
	class MaterialManager;
	class MeshManager;
	class TextureManager;

	class GpuBufferManager;
	class InputDescriptionManager;

	class IGraphicsAPI;

	class Root
	{
		LAG_GENERATE_SINGLETON(Root)

		LAG_GENERATE_OBSERVER_STORAGE(IFrameListener)
		LAG_DECLARE_NOTIFY_METHOD(onFrameStart, LAG_ARGS(float timePassed))
		LAG_DECLARE_NOTIFY_METHOD(onFrameRenderingQueued, LAG_ARGS(float timePassed))
		LAG_DECLARE_NOTIFY_METHOD(onFrameEnd, LAG_ARGS(float timePassed))


	public:
		bool initializeLag(const InitializationParameters &parameters);
		bool initializeLag(const std::string &iniFile);
		
		inline RenderWindow& getRenderWindow() const { return *renderWindow; }
		inline InputManager& getInputManager() const { return *inputManager; }
		inline Renderer& getRenderer() const { return *renderer; }
		inline SceneManager& getSceneManager() const { return *sceneManager; }
		//inline const InitializationParameters& getInitializationParameters() const { return initializationParameters; }

		inline GpuProgramStageManager& getGpuProgramStageManager() const { return *gpuProgramStageManager; }
		inline GpuProgramManager& getGpuProgramManager() const { return *gpuProgramManager; }
		inline MaterialManager& getMaterialManager() const { return *materialManager; }
		inline MeshManager& getMeshManager() const { return *meshManager; }
		inline TextureManager& getTextureManager() const { return *textureManager; }


		inline GpuBufferManager& getGpuBufferManager() const { return *gpuBufferManager; }
		inline InputDescriptionManager& getInputDescriptionManager() const { return *inputDescriptionManager; }

		void startRenderingLoop();
		void stopRenderingLoop();

		void onRenderWindowResize(int width, int height);

	private:
		RenderWindow *renderWindow;
		InputManager *inputManager;
		Renderer *renderer;
		SceneManager *sceneManager;

		GpuProgramStageManager *gpuProgramStageManager;
		GpuProgramManager *gpuProgramManager;
		MaterialManager *materialManager;
		MeshManager *meshManager;
		TextureManager *textureManager;

		GpuBufferManager *gpuBufferManager;
		InputDescriptionManager *inputDescriptionManager;

		IGraphicsAPI *graphicsAPI;

		InitializationParameters initializationParameters;

		//Frame Timing
		Timer wholeFrameTimer;
		Timer frameStartTimer;
		Timer frameEndTimer;
		Timer frameQueuedTimer;

		float minFrameTime; //min frame time to respect the max FPS

		bool shouldLoop;

		void renderOneFrame();
		void destroy();
		bool internalInit(const InitializationParameters &parameters);
		bool initResources(const std::string &resourcesFilePath);

		/*
		* Listen for toggle virtual cursor and escape.
		*/
		class KeyboardListener : public IKeyboardListener
		{
			virtual void onKeyPress(int key, int modifier);
			virtual void onKeyRelease(int key, int modifier) {}
			virtual void onKeyRepeat(int key, int modifier) {}
		};

		/*
		* Listen for window close and stop looping.
		*/
		class WindowListener : public IWindowListener
		{
		public:
			virtual void onPreRender(RenderTarget &notifier) {}
			virtual void onPostRender(RenderTarget &notifier) {}
			virtual void onResize(RenderTarget &notifier, int width, int height) {}
			virtual void onMove(RenderWindow &notifier, int x, int y) {}
			virtual void onFocusChange(RenderWindow &notifier, bool focused) {}
			virtual void onClose(RenderWindow &notifier);
		};

		WindowListener *windowListener;
		KeyboardListener *keyboardListener;
	};
}