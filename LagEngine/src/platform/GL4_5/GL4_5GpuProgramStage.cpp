#include "GL4_5GpuProgramStage.h"

#include "LogManager.h"

using namespace Lag;

GL4GpuProgramStage::GL4GpuProgramStage(const std::string &path, GpuProgramStageType type) :
        GpuProgramStage(path, type) {
}

bool GL4GpuProgramStage::compile() {
    const char *c_str = code.c_str();
    glShaderSource(handle, 1, &c_str, nullptr);
    glCompileShader(handle);
    return checkCompilation();
}

bool GL4GpuProgramStage::checkCompilation() const {
    GLint result;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

    if (!result) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GL4GpuProgramStage", "Failed to compile shader: " + path);

        GLint logLength;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

        auto *log = new GLchar[logLength];
        glGetShaderInfoLog(handle, logLength, nullptr, log);

        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GL4GpuProgramStage", std::string("Compilation Log:\n") + log);

        delete[] log;
        return false;
    }

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                  "GL4GpuProgramStage", "Shader compiled succesfully: " + path);

    return true;
}

GLenum GL4GpuProgramStage::convertTypeToGL() {
    switch (type) {
        case GpuProgramStageType::VERTEX:
            return GL_VERTEX_SHADER;
        case GpuProgramStageType::TESS_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case GpuProgramStageType::TESS_EVALUATION:
            return GL_TESS_EVALUATION_SHADER;
        case GpuProgramStageType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case GpuProgramStageType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case GpuProgramStageType::UNKNOWN:
        default:
            return 0;
    }
}

bool GL4GpuProgramStage::loadImplementation() {
    handle = glCreateShader(convertTypeToGL());
    return GpuProgramStage::loadImplementation();
}

void GL4GpuProgramStage::unloadImplementation() {
    glDeleteShader(handle);
    GpuProgramStage::unloadImplementation();
}
