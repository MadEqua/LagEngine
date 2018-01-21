#pragma once

#include <string>
#include <vector>

#include "../../core/Manager.h"
#include "GpuProgram.h"

namespace Lag
{
	class GpuProgramStage;

	class GpuProgramBuilder : public IManagedObjectBuilder<std::string, GpuProgram>
	{
	public:
		void setStagesNames(std::vector<std::string> &names);
		void setStages(std::vector<GpuProgramStage*> &stages);

	protected:
		std::vector<GpuProgramStage*> stages;
		std::vector<std::string> stagesNames;
		bool useNames;
	};

	
	class GpuProgramManager : public Manager<std::string, GpuProgram>
	{
	public:
		explicit GpuProgramManager(GpuProgramBuilder* builder);
		
		//Convenience methods for the most common operations
		GpuProgram* get(std::vector<std::string> &stagesNames);
		GpuProgram* get(std::vector<std::string> &stagesNames, ManagedObject &parent);

		//Generates a name for the program, combining the names of the stages
		static const std::string generateProgramName(std::vector<std::string> &stageNames);
	};
}