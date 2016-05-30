#include "GL4GpuProgramStage.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

GL4GpuProgramStage::GL4GpuProgramStage(const std::string &path, GpuProgramStageType type) :
	GpuProgramStage(path, type)
{
	GL_ERROR_CHECK(handle = glCreateShader(convertTypeToGL()))
}

GL4GpuProgramStage::~GL4GpuProgramStage()
{
	GL_ERROR_CHECK(glDeleteShader(handle))
}

bool GL4GpuProgramStage::compile()
{
	const char *c_str = code.c_str();
	GL_ERROR_CHECK(glShaderSource(handle, 1, &c_str, 0))

	GL_ERROR_CHECK(glCompileShader(handle))

	return checkCompilation();	
}

bool GL4GpuProgramStage::checkCompilation() const
{
	GLint result;
	GL_ERROR_CHECK(glGetShaderiv(handle, GL_COMPILE_STATUS, &result))

	if (!result)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgramStage", "Failed to compile shader: " + path);

		GLint logLength;
		GL_ERROR_CHECK(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength))
			
		GLchar *log = new GLchar[logLength];
		GL_ERROR_CHECK(glGetShaderInfoLog(handle, logLength, 0, log))

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgramStage", std::string("Compilation Log:\n") + log);

		delete[] log;
		return false;
	}
	
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GL4GpuProgramStage", "Shader compiled succesfully: " + path);

	return true;
}

GLenum GL4GpuProgramStage::convertTypeToGL()
{
	switch (type)
	{
	case LAG_GPU_PROG_STAGE_TYPE_VERTEX:
		return GL_VERTEX_SHADER;
	case LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL:
		return GL_TESS_CONTROL_SHADER;
	case LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION:
		return GL_TESS_EVALUATION_SHADER;
	case LAG_GPU_PROG_STAGE_TYPE_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	case LAG_GPU_PROG_STAGE_TYPE_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case LAG_GPU_PROG_STAGE_TYPE_UNKNOWN:
	default:
		return 0;
	}
}