#include "GpuProgram.h"

#include "GpuProgramStage.h"
#include "../../resources/GpuProgramStageManager.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgram::GpuProgram(const std::vector<std::string> &names)
{
	GpuProgramStageManager &man = Root::getInstance().getGpuProgramStageManager();
	
	std::vector<GpuProgramStage*> stages;
	for (const std::string &name : names) 
	{
		GpuProgramStage *stage = static_cast<GpuProgramStage*>(man.get(name));
		if (stage == nullptr)
			LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::WARNING, 
				"GpuProgram", "Trying to use a non-declared GpuProgramStage: " + name);
		else
			stages.push_back(stage);
	}
	initStages(stages);
}

GpuProgram::GpuProgram(const std::vector<GpuProgramStage*> &stages)
{
	initStages(stages);
}

void GpuProgram::initStages(const std::vector<GpuProgramStage*> &stages)
{
	for (int i = 0; i < PROGRAM_STAGE_COUNT; ++i)
		programStages[i] = nullptr;

	for (GpuProgramStage *stage : stages)
	{
		if (programStages[stage->getType()] != nullptr)
		{
			LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::WARNING, "GpuProgram",
				"Receiving multiple GpuProgramStage for the same stage. Using only the first on list.");
		}
		else
			programStages[stage->getType()] = stage;
	}
}

GpuProgram::~GpuProgram()
{
}

bool GpuProgram::load()
{
	if (programStages[VERTEX] == nullptr)
	{
		LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::ERROR,
			"GpuProgram", "Trying to load without a vertex stage.");
		return false;
	}

	if (!link())
		return false;

	loaded = true;
	return true;
}

void GpuProgram::unload()
{
}