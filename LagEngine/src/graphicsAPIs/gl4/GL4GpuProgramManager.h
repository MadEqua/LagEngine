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
			return new GL4GpuProgram(name, stagesNames);
		}
	};
	
	
	class GL4GpuProgramManager : public GpuProgramManager
	{
	public:
		GL4GpuProgramManager() :
			GpuProgramManager(new GL4GpuProgramBuilder()) {}

		virtual void initialize() override
		{
			std::vector<std::string> defaultStages;
			defaultStages.push_back("defaultVertex");
			defaultStages.push_back("defaultFragment");
			defaultObject = get(defaultStages);
		}
	};
}