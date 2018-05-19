#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../../core/Handle.h"
#include "../../core/ManagedObject.h"
#include "../GpuProgramUniformDescription.h"
#include "../../core/Handle.h"

namespace Lag
{
	class GpuProgramUniform;
	enum GpuProgramStageType;
	enum GpuProgramUniformSemantic;
	class GpuProgramStage;
	
	/*
	* Abstraction of a program meant to be run on the GPU. 
	* Contains a set of GpuProgramStages, only the vertex stage is mandatory.
	* Also creates and maintains Uniforms declared on its Stages.
	*
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgram : public ManagedObject
	{
	public:
		inline bool hasStage(GpuProgramStageType stageType) const { return presentStages[stageType]; }
		
		const GpuProgramUniform* getUniformByName(const std::string &name) const;
		const std::vector<GpuProgramUniform*>* getUniformBySemantic(GpuProgramUniformSemantic semantic) const;

		bool containsStageWithName(const std::string &imageName) const;

	protected:
		GpuProgram(const std::string &name, const std::vector<std::string> &names);

		std::string name;
		std::unordered_map<std::string, GpuProgramUniform*> uniformsByName;
		std::unordered_map<GpuProgramUniformSemantic, std::vector<GpuProgramUniform*>> uniformsBySemantic;

		static const int PROGRAM_STAGE_COUNT = 5;
		bool presentStages[PROGRAM_STAGE_COUNT];
		std::vector<std::string> stagesNames;
		std::vector<Handle<GpuProgramStage>> stages;
		
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		bool checkStages();
		virtual bool link() = 0;
		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description) const = 0;
	};
}