#include "GpuProgramUniform.h"

#include "../GpuProgramUniformDescription.h"

using namespace Lag;

GpuProgramUniform::GpuProgramUniform(const GpuProgramUniformDescription &description, const GpuProgram &gpuProgram) :
	description(description)
{
}

GpuProgramUniform::~GpuProgramUniform()
{
}