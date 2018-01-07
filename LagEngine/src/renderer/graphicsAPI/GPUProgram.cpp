#include "GpuProgram.h"

#include "GpuProgramStage.h"
#include "GpuProgramUniform.h"
#include "../../resources/GpuProgramStageManager.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgram::GpuProgram(const std::string &name, const std::vector<std::string> &names) :
	name(name),
	stagesNames(names)
{
}

GpuProgram::GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages) :
	name(name),
	stages(stages)
{
}

bool GpuProgram::loadImplementation()
{
	if (stages.empty())
	{
		GpuProgramStageManager &man = Root::getInstance().getGpuProgramStageManager();
		for (const std::string &name : stagesNames)
		{
			GpuProgramStage *stage = man.get(name);
			if (stage == nullptr)
				LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
					"GpuProgram", "Trying to use a non-declared GpuProgramStage: " + name);
			else
				stages.push_back(stage);
		}
	}
	if (!checkStages())
		return false;

	if (!link())
		return false;

	for (GpuProgramStage *stage : stages)
	{
		for (uint32 i = 0; i < stage->getUniformDescriptionCount(); ++i)
		{
			const GpuProgramUniformDescription &desc = stage->getUniformDescription(i);
			
			//Check for name/semantics collisions
			if (!uniformsByName.empty())
			{
				auto nameIt = uniformsByName.find(desc.name);
				if (nameIt != uniformsByName.end())
				{
					if (desc.semantic == nameIt->second->getGpuProgramUniformDescription().semantic)
					{
						LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
							"GpuProgram: " + name, "Ignoring one of the Uniforms: " + desc.name +
							". It's declared with the same semantic on more than one Stage.");
						continue;
					}
					else
					{
						LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
							"GpuProgram: " + name, "More than one Uniform with name: " + desc.name +
							"are declared with different semantics on more than one Stage.");
						return false;
					}
				}
				else
				{
					auto semIt = uniformsBySemantic.find(desc.semantic);
					if (semIt != uniformsBySemantic.end())
					{
						LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
							"GpuProgram: " + name, "Declared Uniforms with different names but similar semantics.");
					}
				}
			}
			
			GpuProgramUniform *uniform = createUniform(desc);
			if (uniform != nullptr)
			{
				uniformsByName[desc.name] = uniform;
				uniformsBySemantic[desc.semantic].push_back(uniform);
			}
		}
	}
	return true;
}

bool GpuProgram::checkStages()
{
	for (int i = 0; i < PROGRAM_STAGE_COUNT; ++i)
		presentStages[i] = false;

	for (GpuProgramStage *stage : stages)
	{
		if (presentStages[stage->getType()])
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL, "GpuProgram",
				"Receiving multiple GpuProgramStages for the same stage. Using only the first on list.");
		else
			presentStages[stage->getType()] = true;
	}

	if (!presentStages[LAG_GPU_PROG_STAGE_TYPE_VERTEX])
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GpuProgram", "Trying to load without a vertex stage.");
		return false;
	}
	return true;
}

void GpuProgram::unloadImplementation()
{
	for (auto &un : uniformsByName)
		delete un.second;

	stages.clear();
	uniformsByName.clear();
	uniformsBySemantic.clear();
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

void GpuProgram::generateName(std::vector<std::string> &stageNames, std::string &out)
{
	GpuProgramStageType stageOrder[PROGRAM_STAGE_COUNT] = 
	{ 
		LAG_GPU_PROG_STAGE_TYPE_VERTEX, LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL,
		LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION, LAG_GPU_PROG_STAGE_TYPE_GEOMETRY,
		LAG_GPU_PROG_STAGE_TYPE_FRAGMENT 
	};

	GpuProgramStageManager &man = Root::getInstance().getGpuProgramStageManager();

	for (int i = 0; i < PROGRAM_STAGE_COUNT; ++i)
	{
		for (std::string &stageName : stageNames)
		{
			GpuProgramStage *stage = man.get(stageName);
			if (stage != nullptr && stage->getType() == stageOrder[i])
			{
				out += stageName + '.';
				break;
			}
		}
	}
}