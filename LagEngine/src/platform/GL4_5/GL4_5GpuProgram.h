#pragma once

#include "GpuProgram.h"
#include "GL/glew.h"

namespace Lag {
    class GL4GpuProgram : public GpuProgram {
    public:
        GpuProgramUniform *createUniform(const GpuProgramUniformDescription &description) const override;
        inline GLuint getHandle() const { return handle; }

    protected:
        friend class GL4_5GpuProgramBuilder;

        GL4GpuProgram(const std::string &name, const std::vector<std::string> &names);

        bool link() override;
        bool checkLinking() const;

        bool loadImplementation() override;
        void unloadImplementation() override;

        GLuint handle;
    };
}