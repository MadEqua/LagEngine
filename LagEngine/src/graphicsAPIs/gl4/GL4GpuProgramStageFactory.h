#pragma once

#include "../../resources/GpuProgramStageFactory.h"
#include "GL4GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStageFactory : public GpuProgramStageFactory
	{
	public:
		GL4GpuProgramStageFactory() {}
		virtual ~GL4GpuProgramStageFactory() {}

		virtual Resource* create() const override { return new GL4GpuProgramStage(path, type); }
	};
}