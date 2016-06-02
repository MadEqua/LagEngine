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

void GL4GpuProgramUniform::setValue(const void* value, uint32 arraySize) const
{
	switch (description.type)
	{
		case LAG_GPU_PROG_UNIFORM_TYPE_BOOL:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				return;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_FLOAT:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1fv(location, arraySize, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2fv(location, arraySize, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3fv(location, arraySize, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4fv(location, arraySize, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				return;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_INT32:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4iv(location, arraySize, static_cast<const GLint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				return;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_UINT32:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				GL_ERROR_CHECK(glUniform1uiv(location, arraySize, static_cast<const GLuint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniform2uiv(location, arraySize, static_cast<const GLuint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniform3uiv(location, arraySize, static_cast<const GLuint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniform4uiv(location, arraySize, static_cast<const GLuint*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				return;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_MATRIX:
			switch (description.size)
			{
			case LAG_GPU_PROG_UNIFORM_SIZE_1:
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_2:
				GL_ERROR_CHECK(glUniformMatrix2fv(location, arraySize, GL_FALSE, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_3:
				GL_ERROR_CHECK(glUniformMatrix3fv(location, arraySize, GL_FALSE, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_4:
				GL_ERROR_CHECK(glUniformMatrix4fv(location, arraySize, GL_FALSE, static_cast<const GLfloat*>(value)));
				return;
			case LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN:
			default:
				return;
			}

		case LAG_GPU_PROG_UNIFORM_TYPE_UNKNOWN:
		default:
			return;
	}
}