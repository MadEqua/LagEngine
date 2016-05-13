#include "GpuProgram.h"

#include "GpuProgramStage.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgram::GpuProgram(const std::vector<std::string> &names)
{
	//ResourceManager &rm = Root::getInstance().getResouceManager();
	//TODO
}

GpuProgram::GpuProgram(const std::vector<GpuProgramStage*> &stages)
{
	for (int i = 0; i < PROGRAM_STAGE_COUNT; ++i)
		programStages[i] = nullptr;

	for (GpuProgramStage *stage : stages)
		programStages[stage->getType()] = stage;

	if(programStages[VERTEX] == nullptr)
		LogManager::getInstance().log(FILE, NORMAL, ERROR, "GpuProgram", "Created without vertex stage.");
}

GpuProgram::~GpuProgram()
{
}