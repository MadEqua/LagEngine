#include "GL4_5GpuProgram.h"
#include "GL4_5GpuProgramUniform.h"
#include "GL4_5GpuProgramStage.h"

#include "LogManager.h"


using namespace Lag;

GL4_5GpuProgram::GL4_5GpuProgram(const std::string &name, const std::vector<std::string> &names) :
        GpuProgram(name, names) {
}

bool GL4_5GpuProgram::link() {
    for (auto &stage : stages)
        glAttachShader(handle, dynamic_cast<GL4_5GpuProgramStage *>(stage.get())->getHandle());

    glLinkProgram(handle);
    return checkLinking();
}

bool GL4_5GpuProgram::checkLinking() const {
    GLint result;
    glGetProgramiv(handle, GL_LINK_STATUS, &result);

    if (!result) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GL4_5GpuProgram", "Failed to link shader: " + name);

        GLint logLength;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);

        auto *log = new GLchar[logLength];
        glGetProgramInfoLog(handle, logLength, nullptr, log);

        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GL4_5GpuProgram", std::string("Linking Log:\n") + log);

        delete[] log;
        return false;
    }

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                  "GL4_5GpuProgram", "Shader linked succesfully: " + name);
    return true;
}

bool GL4_5GpuProgram::loadImplementation() {
    handle = glCreateProgram();
    return GpuProgram::loadImplementation();
}

void GL4_5GpuProgram::unloadImplementation() {
    glDeleteProgram(handle);
    return GpuProgram::unloadImplementation();
}

GpuProgramUniform *GL4_5GpuProgram::createUniform(const GpuProgramUniformDescription &description) const {
    return new GL4_5GpuProgramUniform(description, *this);
}