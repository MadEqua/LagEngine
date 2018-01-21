#include "GpuProgramManager.h"

#include <algorithm>

using namespace Lag;

GpuProgramManager::GpuProgramManager(GpuProgramBuilder* builder) :
	Manager("GpuProgramManager", builder)
{
}

GpuProgram* GpuProgramManager::get(std::vector<std::string>& stagesNames)
{
	GpuProgramBuilder &gpuProgramBuilder = static_cast<GpuProgramBuilder&>(*builder);
	gpuProgramBuilder.setStagesNames(stagesNames);
	return Manager::get(generateProgramName(stagesNames));
}

GpuProgram* GpuProgramManager::get(std::vector<std::string> &stagesNames, ManagedObject &parent)
{
	GpuProgramBuilder &gpuProgramBuilder = static_cast<GpuProgramBuilder&>(*builder);
	gpuProgramBuilder.setStagesNames(stagesNames);
	return Manager::get(generateProgramName(stagesNames), parent);
}

void GpuProgramBuilder::setStagesNames(std::vector<std::string> &names)
{
	stagesNames = names;
	useNames = true;
}

void GpuProgramBuilder::setStages(std::vector<GpuProgramStage*> &stages)
{
	this->stages = stages;
	useNames = false;
}

const std::string GpuProgramManager::generateProgramName(std::vector<std::string> &stageNames)
{
	std::sort(stageNames.begin(), stageNames.end());
	std::string out;

	for (std::string &stageName : stageNames)
		out += stageName + '.';
	return out;
}