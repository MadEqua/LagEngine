#pragma once

#include <string>
#include "Types.h"
#include "RenderMode.h"

namespace Lag {
    class GpuProgram;
    class Texture;
    class GpuBuffer;
    class InputDescription;
    class Color;
    struct ImageData;
    class RenderTarget;
    struct BlendingSettings;
    struct DepthSettings;
    enum class RenderMode : uint8;
    enum class BufferType : uint8;
    enum class IndexType : uint8;

    /*
    * Abstracting the calls of a Graphics API. A class implementing this interface is used
    * to interact with the graphics pipeline and issue draw calls.
    */
    class IGraphicsAPI {
    public:
        virtual ~IGraphicsAPI() = default;
        virtual bool initialize() = 0;

        virtual void renderVertices(RenderMode mode, uint32 first, uint32 count) = 0;
        virtual void renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex = 0) = 0;

        /*virtual void renderMultiVertices(RenderMode mode, uint32 first[], uint32 count[], uint32 drawCount) = 0;
        virtual void renderMultiIndexed(RenderMode mode, uint32 first[], uint32 count[], uint32 drawCount) = 0;

        virtual void renderVerticesInstanced(RenderMode mode, uint32 first, uint32 count, uint32 instanceCount) = 0;
        virtual void renderIndexedInstanced(RenderMode mode, uint32 first, uint32 count, uint32 instanceCount) = 0;*/

        virtual void clearColorBuffer(const Color &color) = 0;
        virtual void clearDepthBuffer(float value) = 0;
        virtual void clearStencilBuffer(int32 value) = 0;
        virtual void clearDepthAndStencilBuffer(float depth, int32 stencil) = 0;

        virtual void bindVertexBuffer(const GpuBuffer &vertexBuffer) = 0;
        virtual void bindIndexBuffer(const GpuBuffer &indexBuffer) = 0;
        virtual void bindGpuProgram(const GpuProgram &gpuProgram) = 0;
        virtual void bindInputDescription(const InputDescription &inputDescription) = 0;
        virtual void bindRenderTarget(const RenderTarget &renderTarget) = 0;
        virtual void bindViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void bindTexture(const Texture &texture, uint8 unit = 0) = 0;

        virtual void setPointSizeFromGpuProgramEnabled(bool enabled) = 0;

        virtual void setBlendingSettings(const BlendingSettings &blendingSettings) = 0;
        virtual void setDepthSettings(const DepthSettings &depthSettings) = 0;
    };
}