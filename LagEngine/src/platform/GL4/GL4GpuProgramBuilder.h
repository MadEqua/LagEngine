#pragma once

#include "../../renderer/graphicsAPI/GpuProgramManager.h"

#include "GL4GpuProgram.h"

namespace Lag
{
	class GL4GpuProgramBuilder : public GpuProgramBuilder
	{
	public:
		virtual GpuProgram* build(const std::string &name) const override
		{
			return new GL4GpuProgram(name, stagesNames);
		}
	};
}