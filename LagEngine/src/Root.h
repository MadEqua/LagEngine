#pragma once

#include <string>

#include "InitializationParameters.h"

#include "io/IKeyboardListener.h"
#include "renderer/IWindowListener.h"

#include "core/SingletonPattern.h"

#include "core/Handle.h"
#include "io/ResourceFilesWatcher.h"

class TiXmlDocument;

namespace Lag
{
	class InputManager;
	class Renderer;
	class SceneManager;
	class IFrameListener;
	
	class RenderTargetManager;
	class RenderTarget;

	class GpuProgramStageManager;
	class GpuProgramManager;
	class MaterialManager;
	class MeshManager;
	class ImageManager;
	class TextureManager;

	class GpuBufferManager;
	class InputDescriptionManager;

	class IGraphicsAPI;
	class IPlatformFactory;

	class Root
	{
		LAG_GENERATE_SINGLETON(Root)

	public:
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
		Handle<RenderTarget> renderWindow; //Since this is mandatory, Root will hold into it

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

		ResourceFilesWatcher resourceFilesWatcher;

		void destroy();

		bool initializeLag();
		bool internalInit(const IPlatformFactory *platformFactory);
		bool initResources(const IPlatformFactory *platformFactory, const std::string &resourcesFilePath);

		/*
		* Listen for toggle virtual cursor and escape.
		*/
		class KeyboardListener : public IKeyboardListener
		{
		public:
			virtual void onKeyPress(int key, int modifier) override;
			virtual void onKeyRelease(int key, int modifier) override {}
			virtual void onKeyRepeat(int key, int modifier) override {}
		};

		/*
		* Listen for window close and stop looping.
		*/
		class WindowListener : public IWindowListener
		{
		public:
			virtual void onMove(RenderWindow &notifier, uint32 x, uint32 y) override {}
			virtual void onFocusChange(RenderWindow &notifier, bool focused) override {}
			virtual void onClose(RenderWindow &notifier) override;
		};

		WindowListener windowListener;
		KeyboardListener keyboardListener;
	};
}