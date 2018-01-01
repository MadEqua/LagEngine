#include "GL4GpuProgram.h"
#include "GL4GpuProgramUniform.h"
#include "GL4GpuProgramStage.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

GL4GpuProgram::GL4GpuProgram(const std::string &name, const std::vector<std::string> &names) :
	GpuProgram(name, names)
{
}

GL4GpuProgram::GL4GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages) :
	GpuProgram(name, stages)
{
}

bool GL4GpuProgram::link()
{
	for (GpuProgramStage *stage : stages)
		GL_ERROR_PRINT_BLOCK(glAttachShader(handle, static_cast<GL4GpuProgramStage*>(stage)->getHandle()))
	
	GL_ERROR_PRINT(glLinkProgram(handle))
	return checkLinking();
}

bool GL4GpuProgram::checkLinking() const
{
	GLint result;
	GL_ERROR_PRINT(glGetProgramiv(handle, GL_LINK_STATUS, &result))

	if (!result)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgram", "Failed to link shader: " + name);

		GLint logLength;
		GL_ERROR_PRINT(glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength))

		GLchar *log = new GLchar[logLength];
		GL_ERROR_PRINT(glGetProgramInfoLog(handle, logLength, 0, log))

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgram", std::string("Linking Log:\n") + log);

		delete[] log;
		return false;
	}

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GL4GpuProgram", "Shader linked succesfully: " + name);
	return true;
}

bool GL4GpuProgram::loadImplementation()
{
	GL_ERROR_PRINT(handle = glCreateProgram())
	return GpuProgram::loadImplementation();
}

void GL4GpuProgram::unloadImplementation()
{
	GL_ERROR_PRINT(glDeleteProgram(handle))
	return GpuProgram::unloadImplementation();
}

GpuProgramUniform* GL4GpuProgram::createUniform(const GpuProgramUniformDescription &description) const
{
	return new GL4GpuProgramUniform(description, *this);
}