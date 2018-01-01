#pragma once

#include <string>

#include "../../core/Manager.h"
#include "GpuProgram.h"

namespace Lag
{
	class GpuProgramStage;
	
	class GpuProgramManager : public Manager<std::string, GpuProgram>
	{

	public:
		GpuProgramManager();

		GpuProgram* create(const std::string &name, const std::vector<std::string> &stagesNames);
		GpuProgram* create(const std::string &name, const std::vector<GpuProgramStage*> &stagesNames);

	protected:
		virtual GpuProgram* internalCreate(const std::string &name, const std::vector<std::string> &stagesNames) = 0;
		virtual GpuProgram* internalCreate(const std::string &name, const std::vector<GpuProgramStage*> &stagesNames) = 0;
	};
}