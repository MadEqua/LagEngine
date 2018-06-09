#include "GL4_5GpuProgramUniform.h"
#include "GL4_5GpuProgram.h"
#include "LogManager.h"

using namespace Lag;

GL4GpuProgramUniform::GL4GpuProgramUniform(const GpuProgramUniformDescription &description,
                                           const GpuProgram &gpuProgram) :
        GpuProgramUniform(description, gpuProgram) {

    GLuint programHandle = dynamic_cast<const GL4GpuProgram &>(gpuProgram).getHandle();
    location = glGetUniformLocation(programHandle, description.name.c_str());

    if (location == -1)
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                      "GL4GpuProgramUniform",
                                      "Could not locate an Uniform with name: " + description.name);
}

void GL4GpuProgramUniform::setValue(const void *value, uint32 arraySize) const {
    switch (description.type) {
        case GpuProgramUniformType::BOOL:
            switch (description.size) {
                case GpuProgramUniformSize::SIZE_1:
                    glUniform1iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_2:
                    glUniform2iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_3:
                    glUniform3iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_4:
                    glUniform4iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_UNKNOWN:
                    return;
            }

        case GpuProgramUniformType::FLOAT:
            switch (description.size) {
                case GpuProgramUniformSize::SIZE_1:
                    glUniform1fv(location, arraySize, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_2:
                    glUniform2fv(location, arraySize, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_3:
                    glUniform3fv(location, arraySize, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_4:
                    glUniform4fv(location, arraySize, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_UNKNOWN:
                    return;
            }

        case GpuProgramUniformType::INT32:
            switch (description.size) {
                case GpuProgramUniformSize::SIZE_1:
                    glUniform1iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_2:
                    glUniform2iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_3:
                    glUniform3iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_4:
                    glUniform4iv(location, arraySize, static_cast<const GLint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_UNKNOWN:
                    return;
            }

        case GpuProgramUniformType::UINT32:
            switch (description.size) {
                case GpuProgramUniformSize::SIZE_1:
                    glUniform1uiv(location, arraySize, static_cast<const GLuint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_2:
                    glUniform2uiv(location, arraySize, static_cast<const GLuint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_3:
                    glUniform3uiv(location, arraySize, static_cast<const GLuint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_4:
                    glUniform4uiv(location, arraySize, static_cast<const GLuint *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_UNKNOWN:
                    return;
            }

        case GpuProgramUniformType::MATRIX:
            switch (description.size) {
                case GpuProgramUniformSize::SIZE_1:
                    return;
                case GpuProgramUniformSize::SIZE_2:
                    glUniformMatrix2fv(location, arraySize, GL_FALSE, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_3:
                    glUniformMatrix3fv(location, arraySize, GL_FALSE, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_4:
                    glUniformMatrix4fv(location, arraySize, GL_FALSE, static_cast<const GLfloat *>(value));
                    return;
                case GpuProgramUniformSize::SIZE_UNKNOWN:
                    return;
            }

        case GpuProgramUniformType::UNKNOWN:
            return;
    }
}