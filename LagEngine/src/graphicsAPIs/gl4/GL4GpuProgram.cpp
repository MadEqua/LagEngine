#include "GL4GpuProgram.h"
#include "GL4GpuProgramUniform.h"
#include "GL4GpuProgramStage.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

GL4GpuProgram::GL4GpuProgram(const std::string &name, const std::vector<std::string> &names) :
	GpuProgram(name, names)
{
	GL_ERROR_CHECK(handle = glCreateProgram())
}

GL4GpuProgram::GL4GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages) :
	GpuProgram(name, stages)
{
	GL_ERROR_CHECK(handle = glCreateProgram())
}

GL4GpuProgram::~GL4GpuProgram()
{
	GL_ERROR_CHECK(glDeleteProgram(handle))
}

bool GL4GpuProgram::link()
{
	for (GpuProgramStage *stage : stages)
	{
		GL_ERROR_CHECK(glAttachShader(handle,
			static_cast<GL4GpuProgramStage*>(stage)->getHandle()))
	}
	
	GL_ERROR_CHECK(glLinkProgram(handle))
	return checkLinking();
}

bool GL4GpuProgram::checkLinking() const
{
	GLint result;
	GL_ERROR_CHECK(glGetProgramiv(handle, GL_LINK_STATUS, &result))

	if (!result)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgram", "Failed to link shader: " + name);

		GLint logLength;
		GL_ERROR_CHECK(glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength))

		GLchar *log = new GLchar[logLength];
		GL_ERROR_CHECK(glGetProgramInfoLog(handle, logLength, 0, log))

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgram", std::string("Linking Log:\n") + log);

		delete[] log;
		return false;
	}

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GL4GpuProgram", "Shader linked succesfully: " + name);
	return true;
}

GpuProgramUniform* GL4GpuProgram::createUniform(const GpuProgramUniformDescription &description) const
{
	return new GL4GpuProgramUniform(description, *this);
}