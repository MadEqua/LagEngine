#pragma once

#include "GL4GpuProgram.h"
#include "../../renderer/graphicsAPI/GpuProgramManager.h"

namespace Lag
{
	class GL4GpuProgramManager : public GpuProgramManager
	{

	public:
		virtual GpuProgram* internalCreate(const std::string &name, const std::vector<std::string> &stagesNames) override
		{
			return new GL4GpuProgram(name, stagesNames);
		}

		virtual GpuProgram* internalCreate(const std::string &name, const std::vector<GpuProgramStage*> &stages) override
		{
			return new GL4GpuProgram(name, stages);
		}
	};
}