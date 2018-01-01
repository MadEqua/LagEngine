#include "GpuProgramManager.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgramManager::GpuProgramManager() :
	Manager("GpuProgramManager")
{
}

GpuProgram* GpuProgramManager::create(const std::string &name, const std::vector<std::string> &stagesNames)
{
	GpuProgram *pr = internalCreate(name, stagesNames);
	if (addAndLoad(name, pr))
		return pr;
	else
		return nullptr;
}

GpuProgram* GpuProgramManager::create(const std::string &name, const std::vector<GpuProgramStage*> &stagesNames)
{
	GpuProgram *pr = internalCreate(name, stagesNames);
	if (addAndLoad(name, pr))
		return pr;
	else
		return nullptr;
}
