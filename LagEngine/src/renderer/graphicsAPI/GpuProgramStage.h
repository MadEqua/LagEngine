#pragma once

#include <string>
#include <vector>

#include "Types.h"
#include "XmlResource.h"
#include "GpuProgramUniformDescription.h"

namespace Lag {
    enum class GpuProgramStageType : uint8 {
        VERTEX,
        TESS_CONTROL,
        TESS_EVALUATION,
        GEOMETRY,
        FRAGMENT,
        UNKNOWN
    };

    /*
    * Abstract class representing one of the possible types of shaders that forms a GpuProgram.
    * Can be compiled independently and used on various GpuPrograms.
    * Each GraphicsAPI implementation will have its own concrete version.
    */
    class GpuProgramStage : public XmlResource {
    public:
        void addUniformDescription(const std::string &name, GpuProgramUniformSemantic semantic,
                                   GpuProgramUniformSize size = GpuProgramUniformSize::SIZE_UNKNOWN,
                                   GpuProgramUniformType type = GpuProgramUniformType::UNKNOWN);

        inline GpuProgramStageType getType() const { return type; }

        inline uint32 getUniformDescriptionCount() const { return static_cast<uint32>(uniformDescriptions.size()); }
        inline const GpuProgramUniformDescription &getUniformDescription(int i) const { return uniformDescriptions[i]; }

        virtual bool compile() = 0;

    protected:
        GpuProgramStage(const std::string &filePath, GpuProgramStageType type);

        bool loadImplementation() override;
        void unloadImplementation() override;

        GpuProgramStageType type;
        std::string code;

        //Details of the Uniforms declared for this stage
        std::vector<GpuProgramUniformDescription> uniformDescriptions;
    };
}