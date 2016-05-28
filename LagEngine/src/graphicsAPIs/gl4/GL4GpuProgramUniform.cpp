#include "GL4GpuProgramUniform.h"
#include "GL4GpuProgram.h"
#include "../../renderer/GpuProgramUniformDescription.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GL4GpuProgramUniform::GL4GpuProgramUniform(const GpuProgramUniformDescription &description, const GpuProgram &gpuProgram) :
	GpuProgramUniform(description, gpuProgram)
{
	GLuint programHandle = static_cast<const GL4GpuProgram&>(gpuProgram).getHandle();
	GL_ERROR_CHECK(location = glGetUniformLocation(programHandle, description.name.c_str()))

	if(location == -1)
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GpuProgramUniform", "Could not locate an Uniform with name:" + description.name);
}

GL4GpuProgramUniform::~GL4GpuProgramUniform()
{
}

void GL4GpuProgramUniform::setValue(const void* value) const
{
	switch (description.type)
	{
		case LAG_GPU_PROG_UNIFORM_TYPE_BOOL:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1i(location, *(static_cast<const GLint*>(value))));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2iv(location, 2, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3iv(location, 3, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4iv(location, 4, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				break;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_FLOAT:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1f(location, *(static_cast<const GLfloat*>(value))));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2fv(location, 2, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3fv(location, 3, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4fv(location, 4, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				break;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_INT32:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1i(location, *(static_cast<const GLint*>(value))));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2iv(location, 2, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3iv(location, 3, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4iv(location, 4, static_cast<const GLint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				break;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_UINT32:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1ui(location, *(static_cast<const GLuint*>(value))));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2uiv(location, 2, static_cast<const GLuint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3uiv(location, 3, static_cast<const GLuint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4uiv(location, 4, static_cast<const GLuint*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				break;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_SAMPLER:
			//TODO
			break;

		case LAG_GPU_PROG_UNIFORM_TYPE_MATRIX:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniformMatrix2fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(value)));
				break;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				break;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_UNKNOWN:
		default:
			break;
	}
}