#include "GL4GpuProgram.h"

using namespace Lag;

GL4GpuProgram::GL4GpuProgram(const std::vector<std::string> &names) :
	GpuProgram(names)
{
}

GL4GpuProgram::GL4GpuProgram(const std::vector<GpuProgramStage*> &stages) :
	GpuProgram(stages)
{
}

GL4GpuProgram::~GL4GpuProgram()
{
}

bool GL4GpuProgram::link()
{
	return false;
}

GpuProgramUniform* GL4GpuProgram::createUniform(const GpuProgramUniformDescription &description, void* dataLocation) const
{
	return nullptr;
}