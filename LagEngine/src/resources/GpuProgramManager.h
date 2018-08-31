#pragma once

#include <string>
#include <vector>

#include "Manager.h"
#include "GpuProgram.h"

namespace Lag {
    class GpuProgramStage;

    class GpuProgramBuilder : public IManagedObjectBuilder<std::string, GpuProgram> {
    public:
        std::vector<std::string> stagesNames;
    };


    class GpuProgramManager : public Manager<std::string, GpuProgram> {
    public:
        explicit GpuProgramManager(GpuProgramBuilder *builder);

        Handle<GpuProgram> getFallbackObject() override;

        std::vector<std::string> getProgramsWithStageName(const std::string &stageName) const;

        //Convenience methods for the most common operations
        Handle<GpuProgram> get(std::vector<std::string> &stagesNames);

        //Generates a name for the program, combining the names of the stages
        static const std::string generateProgramName(std::vector<std::string> &stageNames);
    };
}