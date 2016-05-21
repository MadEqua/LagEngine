#pragma once

#include <string>
#include <vector>

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
	class GpuProgram
	{
	public:
		GpuProgram(const std::vector<std::string> &names);
		GpuProgram(const std::vector<GpuProgramStage*> &stages);
		virtual ~GpuProgram();

		//virtual bool load() override;
		//virtual void unload() override;
		
		virtual bool link() = 0;
		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description, void* dataLocation) const = 0;

		inline bool hasStage(GpuProgramStageType stageType) const {return programStages[stageType] != nullptr; }

	private:
		static const int PROGRAM_STAGE_COUNT = 5;
		GpuProgramStage* programStages[PROGRAM_STAGE_COUNT];

		GpuProgramUniforms *uniforms;

		void initStages(const std::vector<GpuProgramStage*> &stages);
	};
}