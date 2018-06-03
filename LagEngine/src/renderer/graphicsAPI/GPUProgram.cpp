#include "GpuProgram.h"

#include <algorithm>

#include "GpuProgramStage.h"
#include "GpuProgramUniform.h"
#include "GpuProgramStageManager.h"
#include "Root.h"
#include "LogManager.h"

using namespace Lag;

GpuProgram::GpuProgram(const std::string &name, const std::vector<std::string> &names) :
        name(name),
        stagesNames(names) {
}

bool GpuProgram::loadImplementation() {
    if (stages.empty()) {
        GpuProgramStageManager &man = Root::getInstance().getGpuProgramStageManager();
        for (const std::string &name : stagesNames) {
            Handle<GpuProgramStage> stage = man.get(name);
            if (!stage.isValid()) //GpuProgramStage have no default. If a stage fails we force error (use the default GpuProgram).
            {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                              "GpuProgram", "Trying to use an invalid GpuProgramStage: " + name);
                return false;
            }
            else
                stages.push_back(stage);
        }
    }

    if (!checkStages())
        return false;

    if (!link())
        return false;

    for (Handle<GpuProgramStage> &stage : stages) {
        for (uint32 i = 0; i < stage->getUniformDescriptionCount(); ++i) {
            const GpuProgramUniformDescription &desc = stage->getUniformDescription(i);

            //Check for name/semantics collisions
            if (!uniformsByName.empty()) {
                auto nameIt = uniformsByName.find(desc.name);
                if (nameIt != uniformsByName.end()) {
                    if (desc.semantic == nameIt->second->getGpuProgramUniformDescription().semantic) {
                        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                                      "GpuProgram: " + name,
                                                      "Ignoring one of the Uniforms: " + desc.name +
                                                      ". It's declared with the same semantic on more than one Stage.");
                        continue;
                    }
                    else {
                        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                                      "GpuProgram: " + name,
                                                      "More than one Uniform with name: " + desc.name +
                                                      "are declared with different semantics on more than one Stage.");
                        return false;
                    }
                }
                else {
                    auto semIt = uniformsBySemantic.find(desc.semantic);
                    if (semIt != uniformsBySemantic.end()) {
                        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                                      "GpuProgram: " + name,
                                                      "Declared Uniforms with different names but similar semantics.");
                    }
                }
            }

            GpuProgramUniform *uniform = createUniform(desc);
            if (uniform != nullptr) {
                uniformsByName[desc.name] = uniform;
                uniformsBySemantic[desc.semantic].push_back(uniform);
            }
        }
    }
    return true;
}

bool GpuProgram::checkStages() {
    for (bool &presentStage : presentStages)
        presentStage = false;

    for (Handle<GpuProgramStage> &stage : stages) {
        if (presentStages[static_cast<int>(stage->getType())])
            LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "GpuProgram",
                                          "Receiving multiple GpuProgramStages for the same stage. Using only the first on list.");
        else
            presentStages[static_cast<int>(stage->getType())] = true;
    }

    if (!presentStages[static_cast<int>(GpuProgramStageType::VERTEX)]) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GpuProgram", "Trying to load without a vertex stage.");
        return false;
    }
    return true;
}

void GpuProgram::unloadImplementation() {
    for (auto &un : uniformsByName)
        delete un.second;

    stages.clear();
    uniformsByName.clear();
    uniformsBySemantic.clear();
}

const GpuProgramUniform *GpuProgram::getUniformByName(const std::string &name) const {
    auto it = uniformsByName.find(name);
    if (it != uniformsByName.end())
        return it->second;
    else
        return nullptr;
}

const std::vector<GpuProgramUniform *> *GpuProgram::getUniformBySemantic(GpuProgramUniformSemantic semantic) const {
    auto it = uniformsBySemantic.find(semantic);
    if (it != uniformsBySemantic.end())
        return &it->second;
    else
        return nullptr;
}

bool GpuProgram::containsStageWithName(const std::string &imageName) const {
    return std::find(stagesNames.begin(), stagesNames.end(), imageName) != stagesNames.end();
}
