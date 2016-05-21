#pragma once

#include "../../resources/GpuProgramStageManager.h"
#include "GL4GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStageManager : public GpuProgramStageManager
	{
	public:
		GL4GpuProgramStageManager() {}
		~GL4GpuProgramStageManager() {}

		virtual void create(const std::string &name, const std::string &file, GpuProgramStageType type) override 
		{
			add(name, new GL4GpuProgramStage(file, type));
		}
	};
}