#include "GpuProgramManager.h"

#include <algorithm>

using namespace Lag;

GpuProgramManager::GpuProgramManager(GpuProgramBuilder *builder) :
        Manager("GpuProgramManager", builder) {
}

Handle<GpuProgram> GpuProgramManager::getFallbackObject() {
    std::vector<std::string> defaultStages;
    defaultStages.emplace_back("defaultVertex");
    defaultStages.emplace_back("defaultFragment");
    return get(defaultStages);
}

std::vector<std::string> GpuProgramManager::getProgramsWithStageName(const std::string &stageName) const {
    std::vector<std::string> res;

    for (auto &pair : objects) {
        GpuProgram &prog = *pair.second;
        if (prog.containsStageWithName(stageName))
            res.push_back(pair.first);
    }
    return res;
}

Handle<GpuProgram> GpuProgramManager::get(std::vector<std::string> &stagesNames) {
    auto &gpuProgramBuilder = dynamic_cast<GpuProgramBuilder &>(*builder);
    gpuProgramBuilder.stagesNames = stagesNames;
    return Manager::get(generateProgramName(stagesNames));
}

const std::string GpuProgramManager::generateProgramName(std::vector<std::string> &stageNames) {
    std::sort(stageNames.begin(), stageNames.end());
    std::string out;

    for (std::string &stageName : stageNames)
        out += stageName + '.';
    return out;
}