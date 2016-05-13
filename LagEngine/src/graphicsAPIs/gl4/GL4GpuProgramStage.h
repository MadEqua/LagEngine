#pragma once

#include "../../renderer/graphicsAPI/GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStage : public GpuProgramStage
	{
	public:
		GL4GpuProgramStage(const std::string &path, GpuProgramStageType type);
		virtual ~GL4GpuProgramStage();

		virtual bool compile() override;
	};
}