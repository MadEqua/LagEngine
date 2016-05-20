#include "GL4GpuProgramUniform.h"

using namespace Lag;

GL4GpuProgramUniform::GL4GpuProgramUniform(uint32 location, GpuProgramUniformSize size,
	GpuProgramUniformType type, GpuProgramUniformSemantic semantic,
	void* dataLocation) :
	GpuProgramUniform(location, size, type, semantic, dataLocation)
{
}

GL4GpuProgramUniform::~GL4GpuProgramUniform()
{
}

void GL4GpuProgramUniform::sendToGpuImpl() const
{

}