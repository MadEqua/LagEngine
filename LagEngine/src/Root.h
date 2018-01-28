#pragma once

#include <string>

#include "InitializationParameters.h"

#include "io/IKeyboardListener.h"
#include "renderer/IWindowListener.h"

#include "core/SingletonPattern.h"

class TiXmlDocument;

namespace Lag
{
	class InputManager;
	class Renderer;
	class SceneManager;
	class IFrameListener;
	
	class RenderTargetManager;

	class GpuProgramStageManager;
	class GpuProgramManager;
	class MaterialManager;
	class MeshManager;
	class ImageManager;
	class TextureManager;

	class GpuBufferManager;
	class InputDescriptionManager;

	class IGraphicsAPI;

	class Root
	{
		LAG_GENERATE_SINGLETON(Root)

	public:
		bool initializeLag(const InitializationParameters &parameters);
		bool initializeLag(const std::string &iniFile);

		void startRenderingLoop();
		void stopRenderingLoop();
		void renderOneFrame();
		
		inline InputManager& getInputManager() const { return *inputManager; }
		inline Renderer& getRenderer() const { return *renderer; }
		inline SceneManager& getSceneManager() const { return *sceneManager; }
		inline RenderTargetManager& getRenderTargetManager() const { return *renderTargetManager; }
		//inline const InitializationParameters& getInitializationParameters() const { return initializationParameters; }

		inline GpuProgramStageManager& getGpuProgramStageManager() const { return *gpuProgramStageManager; }
		inline GpuProgramManager& getGpuProgramManager() const { return *gpuProgramManager; }
		inline MaterialManager& getMaterialManager() const { return *materialManager; }
		inline MeshManager& getMeshManager() const { return *meshManager; }
		inline ImageManager& getImageManager() const { return *imageManager; }
		inline TextureManager& getTextureManager() const { return *textureManager; }

		inline GpuBufferManager& getGpuBufferManager() const { return *gpuBufferManager; }
		inline InputDescriptionManager& getInputDescriptionManager() const { return *inputDescriptionManager; }

		inline const std::string& getResourcesFolder() { return initializationParameters.resourcesFolder; }

	private:
		InputManager *inputManager;
		Renderer *renderer;
		SceneManager *sceneManager;

		RenderTargetManager *renderTargetManager;

		GpuProgramStageManager *gpuProgramStageManager;
		GpuProgramManager *gpuProgramManager;
		MaterialManager *materialManager;
		MeshManager *meshManager;
		ImageManager *imageManager;
		TextureManager *textureManager;

		GpuBufferManager *gpuBufferManager;
		InputDescriptionManager *inputDescriptionManager;

		IGraphicsAPI *graphicsAPI;

		InitializationParameters initializationParameters;
		TiXmlDocument *resourcesFile;

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
			virtual void onMove(RenderWindow &notifier, uint32 x, uint32 y) {}
			virtual void onFocusChange(RenderWindow &notifier, bool focused) {}
			virtual void onClose(RenderWindow &notifier);
		};

		WindowListener *windowListener;
		KeyboardListener *keyboardListener;
	};
}