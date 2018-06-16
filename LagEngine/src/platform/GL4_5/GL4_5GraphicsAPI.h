#pragma once

#include "IGraphicsApi.h"

#include "GL/glew.h"

namespace Lag {
    enum class BlendingFunction : uint8;
    enum class BlendingEquation : uint8;
    enum class ComparisionFunction : uint8;

    class GL4_5GraphicsAPI : public IGraphicsAPI {
    public:
        bool initialize() override;

        void renderVertices(RenderMode mode, uint32 first, uint32 count) override;
        void renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex = 0) override;

        void clearColorBuffer(const Color &color) override;
        void clearDepthBuffer(float value) override;
        void clearStencilBuffer(int32 value) override;
        void clearDepthAndStencilBuffer(float depth, int32 stencil) override;

        void bindVertexBuffer(const GpuBuffer &vertexBuffer) override;
        void bindIndexBuffer(const GpuBuffer &indexBuffer) override;
        void bindGpuProgram(const GpuProgram &gpuProgram) override;
        void bindInputDescription(const InputDescription &inputDescription) override;
        void bindRenderTarget(const RenderTarget &renderTarget) override;
        void bindViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void bindTexture(const Texture &texture, uint8 unit = 0) override;

        void setPointSizeFromGpuProgramEnabled(bool enabled) override;

        void setBlendingSettings(const BlendingSettings &blendingSettings) override;
        void setDepthSettings(const DepthSettings &depthSettings) override ;

    private:
        static GLenum convertRenderModeToGLenum(RenderMode renderMode);
        static GLenum convertComparisionFunctionToGLenum(ComparisionFunction comparisionFunction);
        static GLenum convertBlendingFunctionToGLenum(BlendingFunction blendingFunction);
        static GLenum convertBlendingEquationToGLenum(BlendingEquation blendingEquation);
    };
}