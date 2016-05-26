#pragma once

#include <string>

#include "../../core/Manager.h"

namespace Lag
{
	class GpuProgramStage;
	
	class GpuProgramManager : public Manager<std::string>
	{

	public:
		GpuProgramManager();
		virtual ~GpuProgramManager();

		virtual bool create(const std::string &name, const std::vector<std::string> &stagesNames) = 0;
		virtual bool create(const std::string &name, const std::vector<GpuProgramStage*> &stagesNames) = 0;
	};
}