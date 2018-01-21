#pragma once

#include "GL4GpuProgram.h"
#include "../../renderer/graphicsAPI/GpuProgramManager.h"

namespace Lag
{
	class GL4GpuProgramBuilder : public GpuProgramBuilder
	{
	public:
		virtual GpuProgram* build(const std::string &name) const override
		{
			if(useNames)
				return new GL4GpuProgram(name, stagesNames);
			else
				return new GL4GpuProgram(name, stages);
		}
	};
	
	
	class GL4GpuProgramManager : public GpuProgramManager
	{
	public:
		explicit GL4GpuProgramManager() :
			GpuProgramManager(new GL4GpuProgramBuilder()) {}
	};
}