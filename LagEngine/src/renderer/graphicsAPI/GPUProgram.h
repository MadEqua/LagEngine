#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../core/ManagedObject.h"

namespace Lag
{
	class GpuProgramUniform;
	enum GpuProgramStageType;
	class GpuProgramStage;
	struct GpuProgramUniformDescription;
	
	/*
	* Abstract a program meant to be run on the GPU. 
	* Contains a set of GpuProgramStages, only the vertex stage is mandatory.
	*
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgram : public ManagedObject
	{
	public:
		GpuProgram(const std::string &name, const std::vector<std::string> &names);
		GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages);
		virtual ~GpuProgram();
		
		inline bool hasStage(GpuProgramStageType stageType) const { return presentStages[stageType]; }
		
		GpuProgramUniform *getUniform(const std::string &name) const;

		virtual void bind() const = 0;

	protected:
		std::string name;
		std::unordered_map<std::string, GpuProgramUniform*> uniforms;

		static const int PROGRAM_STAGE_COUNT = 5;
		bool presentStages[PROGRAM_STAGE_COUNT];
		std::vector<GpuProgramStage*> stages;
		
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		virtual bool link() = 0;
		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description, const GpuProgram &program) const = 0;

		void initStages(const std::vector<GpuProgramStage*> &stages);
	};
}