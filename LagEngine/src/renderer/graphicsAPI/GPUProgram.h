#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../core/ManagedObject.h"

namespace Lag
{
	class GpuProgramUniform;
	enum GpuProgramStageType;
	enum GpuProgramUniformSemantic;
	class GpuProgramStage;
	struct GpuProgramUniformDescription;
	
	/*
	* Abstract a program meant to be run on the GPU. 
	* Contains a set of GpuProgramStages, only the vertex stage is mandatory.
	* Also creates and maintains Uniforms declared on its Stages.
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
		
		const GpuProgramUniform* getUniformByName(const std::string &name) const;
		const std::vector<GpuProgramUniform*>* getUniformBySemantic(GpuProgramUniformSemantic semantic) const;

		//Generates a name for the program, combining the names of the stages on a predefined order
		static void generateName(std::vector<std::string> &stageNames, std::string &out);

	protected:
		std::string name;
		std::unordered_map<std::string, GpuProgramUniform*> uniformsByName;
		std::unordered_map<GpuProgramUniformSemantic, std::vector<GpuProgramUniform*>> uniformsBySemantic;

		static const int PROGRAM_STAGE_COUNT = 5;
		bool presentStages[PROGRAM_STAGE_COUNT];
		std::vector<std::string> stagesNames;
		std::vector<GpuProgramStage*> stages;
		
		bool checkStages();

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		virtual bool link() = 0;
		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description) const = 0;
	};
}