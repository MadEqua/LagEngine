#pragma once

#include <string>

#include "ManagedObject.h"

#include "NamedContainer.h"
#include "ObserverPattern.h"
#include "Types.h"
#include "Viewport.h"
#include "RenderPhase.h"
#include "IRenderTargetListener.h"

namespace Lag {
    class Camera;
    class RenderQueue;
    class SceneManager;

    class RenderTarget : public ManagedObject {

    LAG_GENERATE_OBSERVER_STORAGE(IRenderTargetListener)
    LAG_GENERATE_NOTIFY_METHOD(onPreRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
    LAG_GENERATE_NOTIFY_METHOD(onPostRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
    LAG_GENERATE_NOTIFY_METHOD(onResize, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier, uint32 width, uint32 height), LAG_ARGS(notifier, width, height))

    public:
        RenderTarget(uint32 width, uint32 height, RenderPhase renderPhase = RenderPhase::COLOR, bool isMainWindow = false);

        virtual void resize(uint32 width, uint32 height);
        void addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager);

        //For double-buffered targets
        virtual void swapBuffers() {}

        Viewport &createViewport(Camera &camera, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
        Viewport *getViewport(uint16 name) const;
        void clearViewports();

        inline uint32 getWidth() const { return width; }
        inline uint32 getHeight() const { return height; }

        inline bool isMainWindow() const { return mainWindow; }
        inline RenderPhase getRenderPhase() const { return renderPhase; }
        inline void notifyPostRender() { onPostRenderNotify(*this); }

        //TODO (fps, frametimes...)
        //FrameMetrics frameMetrics;

    protected:
        NamedContainer<Viewport> viewports;

        uint32 width, height;
        bool mainWindow;

        RenderPhase renderPhase;
    };
}