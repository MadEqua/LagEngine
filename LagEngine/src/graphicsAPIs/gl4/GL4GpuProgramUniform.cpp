#include "GL4GpuProgramUniform.h"

using namespace Lag;

GL4GpuProgramUniform::GL4GpuProgramUniform(const GpuProgramUniformDescription &description, void* dataLocation) :
	GpuProgramUniform(description, dataLocation)
{
}

GL4GpuProgramUniform::~GL4GpuProgramUniform()
{
}

void GL4GpuProgramUniform::sendToGpuImpl() const
{

}