#pragma once

#include "../../renderer/graphicsAPI/GpuProgram.h"

namespace Lag
{
	class GL4GpuProgram : public GpuProgram
	{
	public:
		GL4GpuProgram();
		virtual ~GL4GpuProgram();
	};
}