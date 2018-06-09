#pragma once

#include <string>

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
        //inline const InitializationParameters& getInitializationParameters() const { return initializationParameters; }
        inline GpuProgramStageManager &getGpuProgramStageManager() const { return *gpuProgramStageManager; }
        inline GpuProgramManager &getGpuProgramManager() const { return *gpuProgramManager; }
        inline MaterialManager &getMaterialManager() const { return *materialManager; }
        inline MeshManager &getMeshManager() const { return *meshManager; }
        inline ImageManager &getImageManager() const { return *imageManager; }
        inline TextureManager &getTextureManager() const { return *textureManager; }
        inline GpuBufferManager &getGpuBufferManager() const { return *gpuBufferManager; }
        inline InputDescriptionManager &getInputDescriptionManager() const { return *inputDescriptionManager; }

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

        InitializationParameters *initializationParameters;
        TiXmlDocument *appResourcesFile;
        TiXmlDocument *lagResourcesFile;

#ifdef ENABLE_DEBUG_MACRO
        ResourceFilesWatcher *resourceFilesWatcher;
#endif

        void destroy();

        bool internalInit(const IPlatformFactory *platformFactory);
        bool initResources(const IPlatformFactory *platformFactory);
        bool initResourcesFiles();
        bool checkResourcesFile(const std::string &filePath, TiXmlDocument *resourcesFile) const;

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