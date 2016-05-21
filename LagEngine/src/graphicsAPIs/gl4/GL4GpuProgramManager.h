#pragma once

#include "GL4GpuProgram.h"
#include "../../renderer/graphicsAPI/GpuProgramManager.h"

namespace Lag
{
	class GL4GpuProgramManager : public GpuProgramManager
	{

	public:
		GL4GpuProgramManager() {}
		virtual ~GL4GpuProgramManager() {}

		virtual void create(const std::string &name, const std::vector<std::string> &stagesNames) override
		{
			add(name, new GL4GpuProgram(stagesNames));
		}

		virtual void create(const std::string &name, const std::vector<GpuProgramStage*> &stages) override
		{
			add(name, new GL4GpuProgram(stages));
		}
	};
}