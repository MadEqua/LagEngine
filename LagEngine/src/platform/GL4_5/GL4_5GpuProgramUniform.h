#pragma once

#include "GpuProgramUniform.h"
#include "GL/glew.h"

namespace Lag {
    class GL4_5GpuProgramUniform : public GpuProgramUniform {
    public:
        GL4_5GpuProgramUniform(const GpuProgramUniformDescription &description, const GpuProgram &gpuProgram);

        void setValue(const void *value, uint32 arraySize) const override;

    protected:
        GLint location;
    };
}