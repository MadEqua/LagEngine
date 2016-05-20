#pragma once

#include "graphicsAPI/GpuProgram.h"
#include "../core/Manager.h"

namespace Lag
{
	class GpuProgramManager : public Manager<GpuProgram>
	{

	public:
		GpuProgramManager(IFactory<GpuProgram> *factory) : Manager(factory) {}
		virtual ~GpuProgramManager() {}
	};
}