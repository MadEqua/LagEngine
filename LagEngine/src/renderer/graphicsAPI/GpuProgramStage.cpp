#include "GpuProgramStage.h"

#include "LogManager.h"
#include <fstream>

using namespace Lag;

GpuProgramStage::GpuProgramStage(const std::string &filePath, GpuProgramStageType type) :
        XmlResource(filePath),
        type(type) {
}

void GpuProgramStage::addUniformDescription(const std::string &name, GpuProgramUniformSemantic semantic,
                                            GpuProgramUniformSize size, GpuProgramUniformType type) {
    GpuProgramUniformDescription desc;
    desc.name = name;
    desc.size = size;
    desc.type = type;
    desc.semantic = semantic;
    uniformDescriptions.push_back(desc);
}

bool GpuProgramStage::loadImplementation() {
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GpuProgramStage", "Cannot open shader file: " + path);
        return false;
    }

    code.clear();
    std::string str;
    while (std::getline(file, str))
        code += str + '\n';

    file.close();

    return compile();
}

void GpuProgramStage::unloadImplementation() {
}