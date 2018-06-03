#pragma once

#include "GpuProgramStage.h"
#include "GL/glew.h"

namespace Lag {
    class GL4GpuProgramStage : public GpuProgramStage {
    public:
        bool compile() override;
        inline GLuint getHandle() const { return handle; }

    private:
        friend class GL4_5GpuProgramStageBuilder;

        GL4GpuProgramStage(const std::string &path, GpuProgramStageType type);

        GLuint handle;
        GLenum convertTypeToGL();

        bool loadImplementation() override;
        void unloadImplementation() override;

        bool checkCompilation() const;
    };
}