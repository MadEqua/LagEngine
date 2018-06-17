#pragma once

#include <string>
#include <unordered_map>

#include "RenderPhase.h"
#include "NamedContainer.h"
#include "Types.h"
#include "Constants.h"
#include "RenderQueue.h"
#include "Color.h"
#include "IRenderTargetListener.h"
#include "ObserverPattern.h"
#include "GpuProgramUniformFiller.h"
#include "Timer.h"
#include "RenderTargetManager.h"
#include "IFrameListener.h"

namespace Lag {

    class RenderTarget;
    class RenderWindow;
    class RenderToTexture;
    class RenderWindow;
    class IGraphicsAPI;
    class SceneManager;
    class GpuProgram;
    class GpuBuffer;
    class InputDescription;
    class Viewport;
    class Texture;
    struct BlendingSettings;
    struct DepthSettings;
    enum class TextureType : uint8;
    enum class RenderMode : uint8;

    /*
    * Encapsulating all the texture bindings to the various binding points.
    */
    class TextureBindings {
    public:
        void setAsBound(const Texture &tex, uint8 unit);
        const Texture *getBinding(uint8 unit) const;
        void reset();

        std::unordered_map<uint8, const Texture *> bindings;
    };

    /*
    * Top level renderer. All the rendering process starts here culminating on concrete calls to a IGraphicsAPI.
    *
    * TODO: describe the process...
    */
    class Renderer : public IRenderTargetListener {
        LAG_GENERATE_OBSERVER_STORAGE(IFrameListener)
        LAG_GENERATE_NOTIFY_METHOD(onFrameStart, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
        LAG_GENERATE_NOTIFY_METHOD(onFrameRenderingQueued, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
        LAG_GENERATE_NOTIFY_METHOD(onFrameEnd, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))

    public:
        Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager, RenderTargetManager &renderTargetManager);
        ~Renderer() override;

        //Entry points for rendering
        void startRenderingLoop(uint32 maxFps);
        void stopRenderingLoop();
        void renderOneFrame();

        //Bind objects and settings
        void bindVertexBuffer(const GpuBuffer &vertexBuffer);
        void bindIndexBuffer(const GpuBuffer &indexBuffer);
        void bindGpuProgram(const GpuProgram &gpuProgram);
        void bindInputDescription(const InputDescription &inputDescription);
        void bindRenderTarget(const RenderTarget &renderTarget);
        void bindViewport(const Viewport &viewport);
        void bindTexture(const Texture &texture, uint8 unit = 0);
        void bindDepthSettings(const DepthSettings &depthSettings);
        void bindBlendingSettings(const BlendingSettings &blendingSettings);

        void resetToBasicState();


        //High-level render calls
        void renderVertices(RenderMode renderMode, const VertexData &vertexData);
        void renderIndexed(RenderMode renderMode, const VertexData &vertexData, const IndexData &indexData);

        /*void renderMultiVertices(const VertexData *vertexData[], uint32 drawCount);
        void renderMultiIndexed(const VertexData *vertexData[], const IndexData *indexData[], uint32 drawCount);

        void renderVerticesInstanced(const VertexData &vertexData, uint32 instanceCount);
        void renderIndexedInstanced(const VertexData &vertexData, const IndexData &indexData, uint32 instanceCount);*/

        void setClearColor(const Color &color);
        void setClearDepthValue(float value);
        void setClearStencilValue(int32 value);

        void clearColorBuffer();
        void clearDepthBuffer();
        void clearStencilBuffer();
        void clearDepthAndStencilBuffer();

        void setPointSizeFromGpuProgramEnabled(bool enabled);

        inline GpuProgramUniformFiller &getUniformFiller() { return uniformFiller; }
        inline float getTotalSeconds() { return totalSeconds; }

    private:
        bool shouldLoop;
        uint64 actualFrame;

        //Frame Timing
        Timer wholeFrameTimer;
        Timer frameStartTimer;
        Timer frameEndTimer;
        Timer frameQueuedTimer;

        //To pass to Shaders with an animation timer
        Timer totalTimer;
        float totalSeconds;

        SceneManager &sceneManager;
        IGraphicsAPI &graphicsAPI;
        RenderTargetManager &renderTargetManager;

        GpuProgramUniformFiller uniformFiller;
        RenderQueue renderQueue;

        Color clearColor;
        float depthClearValue;
        int32 stencilClearValue;

        //TODO: uint32 batch, face and vx counter

        //Bound objects
        const GpuBuffer *boundVertexBuffer;
        const GpuBuffer *boundIndexBuffer;
        const GpuProgram *boundGpuProgram;
        const InputDescription *boundInputDescription;
        const RenderTarget *boundRenderTarget;
        const Viewport *boundViewport;
        const DepthSettings *boundDepthSettings;
        const BlendingSettings *boundBlendingSettings;
        TextureBindings boundTextures;

        //This exists for the case where a frame finishes with the same GpuProgram than the next one will start
        //Force uniformFiller.onGpuProgramBind() to be called in that case, some uniforms may need to be updated.
        const GpuProgram *lastUsedGpuProgramOnFrame;

        void onPreRender(RenderTarget &notifier) override {}
        void onPostRender(RenderTarget &notifier) override {}
        void onResize(RenderTarget &notifier, uint32 width, uint32 height) override;
    };
}