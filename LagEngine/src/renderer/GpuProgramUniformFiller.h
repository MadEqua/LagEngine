#pragma once

#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "Types.h"

namespace Lag {
    class GpuProgram;
    class Viewport;
    class Texture;
    enum class GpuProgramUniformSemantic : uint8;
    class TextureBindings;

    /*
    * This class fills the uniforms with Engine-defined semantics.
    * The Renderer calls the appropriate methods to only send new data when it's actually needed.
    * A Renderable can also rely on this class to quickly fill all the needed Uniforms.
    */
    class GpuProgramUniformFiller {
    public:
        GpuProgramUniformFiller();

        void onGpuProgramBind(const GpuProgram *gpuProgram, const Viewport *viewport,
                              const TextureBindings &textureBindings);
        void onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport);
        void onTextureBind(const GpuProgram *gpuProgram, const Texture *texture, uint8 unit);

        void onRenderableRender(const GpuProgram &gpuProgram, const Viewport &viewportconst,
                                const glm::mat4 &modelMatrix = glm::mat4(1.0f),
                                const glm::mat3 &normalMatrix = glm::mat3(1.0f));

    private:
        void updateLightUniforms(const GpuProgram &gpuProgram);
        void updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport);

        bool programContainsUniform(const GpuProgram &gpuProgram,
                                    GpuProgramUniformSemantic semantic);

        void setUniformIfPresent(const GpuProgram &gpuProgram,
                                 GpuProgramUniformSemantic semantic, const void *value, uint32 arraySize = 1);

        const GpuProgram *frameEndGpuProgram;
    };
}