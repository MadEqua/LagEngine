#pragma once

#include "../../renderer/graphicsAPI/GpuProgram.h"

namespace Lag
{
	class GL4GpuProgram : public GpuProgram
	{
	public:
		GL4GpuProgram(const std::vector<std::string> &names);
		GL4GpuProgram(const std::vector<GpuProgramStage*> &stages);
		virtual ~GL4GpuProgram();

		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description, void* dataLocation) const override;

	protected:
		virtual bool link() override;
	};
}