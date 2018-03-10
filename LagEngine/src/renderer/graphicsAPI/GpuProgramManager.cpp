#include "GpuProgramManager.h"

#include <algorithm>

using namespace Lag;

GpuProgramManager::GpuProgramManager(GpuProgramBuilder* builder) :
	Manager("GpuProgramManager", builder)
{
}

void GpuProgramManager::initializeFallbackObject()
{
	std::vector<std::string> defaultStages;
	defaultStages.push_back("defaultVertex");
	defaultStages.push_back("defaultFragment");
	fallbackObject = get(defaultStages);
}

Handle<GpuProgram> GpuProgramManager::get(std::vector<std::string>& stagesNames)
{
	GpuProgramBuilder &gpuProgramBuilder = static_cast<GpuProgramBuilder&>(*builder);
	gpuProgramBuilder.stagesNames = stagesNames;
	return Manager::get(generateProgramName(stagesNames));
}

const std::string GpuProgramManager::generateProgramName(std::vector<std::string> &stageNames)
{
	std::sort(stageNames.begin(), stageNames.end());
	std::string out;

	for (std::string &stageName : stageNames)
		out += stageName + '.';
	return out;
}