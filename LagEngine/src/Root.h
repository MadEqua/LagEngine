#pragma once

#include <string>
#include <memory>

#include "InitializationParameters.h"

#include "IKeyboardListener.h"
#include "IWindowListener.h"

#include "SingletonPattern.h"

#include "Handle.h"

class TiXmlDocument;

namespace Lag {

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
    class ResourceFilesWatcher;

    class Root {
        LAG_GENERATE_SINGLETON(Root)

    public:
        bool initializeLag(const std::string &iniFile);

        void startRenderingLoop();
        void stopRenderingLoop();
        void renderOneFrame();

        void reloadResourcesFile();

        inline InputManager &getInputManager() const { return *inputManager; }
        inline Renderer &getRenderer() const { return *renderer; }
        inline SceneManager &getSceneManager() const { return *sceneManager; }
        inline RenderTargetManager &getRenderTargetManager() const { return *renderTargetManager; }
        inline GpuProgramStageManager &getGpuProgramStageManager() const { return *gpuProgramStageManager; }
        inline GpuProgramManager &getGpuProgramManager() const { return *gpuProgramManager; }
        inline MaterialManager &getMaterialManager() const { return *materialManager; }
        inline MeshManager &getMeshManager() const { return *meshManager; }
        inline ImageManager &getImageManager() const { return *imageManager; }
        inline TextureManager &getTextureManager() const { return *textureManager; }
        inline GpuBufferManager &getGpuBufferManager() const { return *gpuBufferManager; }
        inline InputDescriptionManager &getInputDescriptionManager() const { return *inputDescriptionManager; }
        inline const TiXmlDocument& getAppResourcesFile() const { return *appResourcesFile; }
        inline const TiXmlDocument& getLagResourcesFile() const { return *lagResourcesFile; }

    private:
        bool initialized;

        //This order of declarations is important for the destruction.
        std::unique_ptr<RenderTargetManager> renderTargetManager;
        Handle<RenderTarget> renderWindow; //Since this is mandatory, Root will hold into it

        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<IGraphicsAPI> graphicsAPI;

        std::unique_ptr<InputDescriptionManager> inputDescriptionManager;
        std::unique_ptr<GpuBufferManager> gpuBufferManager;
        std::unique_ptr<MeshManager> meshManager;
        std::unique_ptr<ImageManager> imageManager;
        std::unique_ptr<GpuProgramStageManager> gpuProgramStageManager;
        std::unique_ptr<TextureManager> textureManager;
        std::unique_ptr<GpuProgramManager> gpuProgramManager;
        std::unique_ptr<MaterialManager> materialManager;
        std::unique_ptr<SceneManager> sceneManager;

        std::unique_ptr<InitializationParameters> initializationParameters;
        std::unique_ptr<TiXmlDocument> appResourcesFile;
        std::unique_ptr<TiXmlDocument> lagResourcesFile;

#ifdef ENABLE_DEBUG_MACRO
        std::unique_ptr<ResourceFilesWatcher> resourceFilesWatcher;
#endif

        bool internalInit(const IPlatformFactory &platformFactory);
        bool initResources(const IPlatformFactory &platformFactory);
        bool initResourcesFiles();
        bool checkResourcesFile(const std::string &filePath, TiXmlDocument &resourcesFile) const;

        /*
        * Listen for toggle virtual cursor and escape.
        */
        class KeyboardListener : public IKeyboardListener {
        public:
            void onKeyPress(int key, int modifier) override;
            void onKeyRelease(int key, int modifier) override {}
            void onKeyRepeat(int key, int modifier) override {}
        };

        /*
        * Listen for window close and stop looping.
        */
        class WindowListener : public IWindowListener {
        public:
            void onMove(RenderWindow &notifier, uint32 x, uint32 y) override {}
            void onFocusChange(RenderWindow &notifier, bool focused) override {}
            void onClose(RenderWindow &notifier) override;
        };

        WindowListener windowListener;
        KeyboardListener keyboardListener;
    };
}