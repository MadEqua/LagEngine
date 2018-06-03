#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Types.h"

#include "Handle.h"
#include "ManagedObject.h"
#include "GpuProgramUniformDescription.h"

#include "GpuProgramStage.h"
#include "GpuProgramUniform.h"

namespace Lag {
    enum class GpuProgramUniformSemantic : uint8;

    /*
    * Abstraction of a program meant to be run on the GPU.
    * Contains a set of GpuProgramStages, only the vertex stage is mandatory.
    * Also creates and maintains Uniforms declared on its Stages.
    *
    * Each GraphicsAPI implementation will have its own concrete version.
    */
    class GpuProgram : public ManagedObject {
    public:
        inline bool hasStage(GpuProgramStageType stageType) const { return presentStages[static_cast<int>(stageType)]; }

        const GpuProgramUniform *getUniformByName(const std::string &name) const;
        const std::vector<GpuProgramUniform *> *getUniformBySemantic(GpuProgramUniformSemantic semantic) const;

        bool containsStageWithName(const std::string &imageName) const;

    protected:
        GpuProgram(const std::string &name, const std::vector<std::string> &names);

        std::string name;
        std::unordered_map<std::string, GpuProgramUniform *> uniformsByName;
        std::unordered_map<GpuProgramUniformSemantic, std::vector<GpuProgramUniform *>> uniformsBySemantic;

        static const int PROGRAM_STAGE_COUNT = 5;
        bool presentStages[PROGRAM_STAGE_COUNT];
        std::vector<std::string> stagesNames;
        std::vector<Handle<GpuProgramStage>> stages;

        bool loadImplementation() override;
        void unloadImplementation() override;

        bool checkStages();
        virtual bool link() = 0;
        virtual GpuProgramUniform *createUniform(const GpuProgramUniformDescription &description) const = 0;
    };
}