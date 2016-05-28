#include "GpuProgram.h"

#include "GpuProgramStage.h"
#include "GpuProgramUniform.h"
#include "../../resources/GpuProgramStageManager.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgram::GpuProgram(const std::string &name, const std::vector<std::string> &names) :
	name(name)
{
	GpuProgramStageManager &man = Root::getInstance().getGpuProgramStageManager();
	
	std::vector<GpuProgramStage*> stages;
	for (const std::string &name : names) 
	{
		GpuProgramStage *stage = static_cast<GpuProgramStage*>(man.get(name));
		if (stage == nullptr)
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				"GpuProgram", "Trying to use a non-declared GpuProgramStage: " + name);
		else
			stages.push_back(stage);
	}
	initStages(stages);
}

GpuProgram::GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages) :
	name(name)
{
	initStages(stages);
}

void GpuProgram::initStages(const std::vector<GpuProgramStage*> &stages)
{
	for (int i = 0; i < PROGRAM_STAGE_COUNT; ++i)
		presentStages[i] = false;

	for (GpuProgramStage *stage : stages)
	{
		if (presentStages[stage->getType()])
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL, "GpuProgram",
				"Receiving multiple GpuProgramStages for the same stage. Using only the first on list.");
		else
		{
			presentStages[stage->getType()] = true;
			this->stages.push_back(stage);
		}
	}
}

GpuProgram::~GpuProgram()
{
	for (auto &un : uniformsByName)
		delete un.second;
}

bool GpuProgram::loadImplementation()
{
	if (!presentStages[LAG_GPU_PROG_STAGE_TYPE_VERTEX])
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GpuProgram", "Trying to load without a vertex stage.");
		return false;
	}

	if (!link())
		return false;

	for (GpuProgramStage *stage : stages)
	{
		for (int i = 0; i < stage->getUniformDescriptionCount(); ++i)
		{
			const GpuProgramUniformDescription &desc = stage->getUniformDescription(i);
			GpuProgramUniform *uniform = createUniform(desc, *this);
			if (uniform != nullptr)
			{
				uniformsByName[desc.name] = uniform;
				uniformsBySemantic[desc.semantic].push_back(uniform);
			}
		}
	}
	return true;
}

void GpuProgram::unloadImplementation()
{
}

const GpuProgramUniform* GpuProgram::getUniformByName(const std::string &name) const
{
	auto it = uniformsByName.find(name);
	if (it != uniformsByName.end())
		return it->second;
	else
		return nullptr;
}

const std::vector<GpuProgramUniform*>* GpuProgram::getUniformBySemantic(GpuProgramUniformSemantic semantic) const
{
	auto it = uniformsBySemantic.find(semantic);
	if (it != uniformsBySemantic.end())
		return &it->second;
	else
		return nullptr;
}
