#pragma once

#include <string>
#include <vector>

#include "../../resources/Resource.h"

namespace Lag
{
	enum GpuProgramStageType;
	class GpuProgramStage;
	
	/*
	* Abstract a program meant to be run on the GPU. 
	* Contains a set of GpuProgramStages, only the vertex stage is mandatory.
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgram : public Resource
	{
	public:
		GpuProgram(const std::vector<std::string> &names);
		GpuProgram(const std::vector<GpuProgramStage*> &stages);
		virtual ~GpuProgram();
		
		virtual void link() = 0;

		inline bool hasStage(GpuProgramStageType stageType) const {return programStages[stageType] != nullptr; }

	private:
		static const int PROGRAM_STAGE_COUNT = 5;
		GpuProgramStage* programStages[PROGRAM_STAGE_COUNT];
	};
}