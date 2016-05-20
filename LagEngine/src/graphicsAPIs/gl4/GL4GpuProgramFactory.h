#pragma once

#include "../../renderer/graphicsAPI/GpuProgramFactory.h"
#include "GL4GpuProgram.h"

namespace Lag
{
	class GL4GpuProgramFactory : public GpuProgramFactory
	{
	public:
		GL4GpuProgramFactory() {}
		virtual ~GL4GpuProgramFactory() {}

		virtual GpuProgram* create() const override { return new GL4GpuProgram(stagesNames); }
	};
}