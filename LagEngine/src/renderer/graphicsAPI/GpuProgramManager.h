#pragma once

#include <vector>
#include <string>

#include "GpuProgram.h"
#include "../../core/Manager.h"

namespace Lag
{
	class GpuProgramStage;
	
	class GpuProgramManager : public Manager<std::string, GpuProgram>
	{

	public:
		GpuProgramManager() {}
		virtual ~GpuProgramManager() {}

		virtual void create(const std::string &name, const std::vector<std::string> &stagesNames) = 0;
		virtual void create(const std::string &name, const std::vector<GpuProgramStage*> &stagesNames) = 0;
	};
}