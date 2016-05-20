#pragma once

#include <string>
#include <vector>

#include "../../core/IFactory.h"

namespace Lag
{
	class GpuProgram;
	class GpuProgramStage;
	
	class GpuProgramFactory : public IFactory<GpuProgram>
	{
	public:
		GpuProgramFactory() {};
		virtual ~GpuProgramFactory() {};

		virtual GpuProgram* create() const = 0;

		std::vector<std::string> stagesNames;
	};
}