#pragma once

#include <string>
#include <vector>
#include "../../core/ManagedObject.h"

namespace Lag
{
	class GpuProgramUniform;
	struct GpuProgramUniformDescription;
	enum GpuProgramStageType;
	class GpuProgramStage;
	class GpuProgramUniforms;
	
	/*
	* Abstract a program meant to be run on the GPU. 
	* Contains a set of GpuProgramStages, only the vertex stage is mandatory.
	*
	* Each GraphicsAPI implementation will have its own concrete version.
	* which has the responsability of buildidng concrete GpuProgramUniforms also.
	*/
	class GpuProgram : public ManagedObject
	{
	public:
		GpuProgram(const std::vector<std::string> &names);
		GpuProgram(const std::vector<GpuProgramStage*> &stages);
		virtual ~GpuProgram();

		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description, void* dataLocation) const = 0;
		
		inline bool hasStage(GpuProgramStageType stageType) const {return programStages[stageType] != nullptr; }

	protected:

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		virtual bool link() = 0;

		static const int PROGRAM_STAGE_COUNT = 5;
		std::vector<GpuProgramStage*> programStages;

		GpuProgramUniforms *uniforms;

		void initStages(const std::vector<GpuProgramStage*> &stages);
	};
}