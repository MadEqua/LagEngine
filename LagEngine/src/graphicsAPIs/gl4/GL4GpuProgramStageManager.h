#pragma once

#include "../../resources/GpuProgramStageManager.h"
#include "GL4GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStageManager : public GpuProgramStageManager
	{
	public:
		GL4GpuProgramStageManager(const std::string &folder) : GpuProgramStageManager(folder) {}

		virtual GpuProgramStage* internalCreate(const std::string &file, GpuProgramStageType type) override
		{	
			return new GL4GpuProgramStage(folder + '/' + file, type);
		}
	};
}