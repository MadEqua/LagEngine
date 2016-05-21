#include "GL4GpuProgramStage.h"

using namespace Lag;

GL4GpuProgramStage::GL4GpuProgramStage(const std::string &path, GpuProgramStageType type) :
	GpuProgramStage(path, type)
{
}

GL4GpuProgramStage::~GL4GpuProgramStage()
{
}

bool GL4GpuProgramStage::compile()
{
	return true;
}