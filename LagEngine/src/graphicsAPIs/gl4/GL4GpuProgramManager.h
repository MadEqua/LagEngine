#pragma once

#include "GL4GpuProgram.h"
#include "../../renderer/graphicsAPI/GpuProgramManager.h"

namespace Lag
{
	class GL4GpuProgramManager : public GpuProgramManager
	{

	public:
		virtual bool create(const std::string &name, const std::vector<std::string> &stagesNames) override
		{
			return add(name, new GL4GpuProgram(name, stagesNames));
		}

		virtual bool create(const std::string &name, const std::vector<GpuProgramStage*> &stages) override
		{
			return add(name, new GL4GpuProgram(name, stages));
		}
	};
}