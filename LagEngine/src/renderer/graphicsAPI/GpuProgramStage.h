#pragma once

#include <string>
#include <vector>

#include "../../resources/Resource.h"
#include "../GpuProgramUniformDescription.h"

namespace Lag
{
	enum GpuProgramStageType
	{
		LAG_GPU_PROG_STAGE_TYPE_VERTEX,
		LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL,
		LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION,
		LAG_GPU_PROG_STAGE_TYPE_GEOMETRY,
		LAG_GPU_PROG_STAGE_TYPE_FRAGMENT,
		LAG_GPU_PROG_STAGE_TYPE_UNKNOWN
	};
	
	/*
	* Abstract class representing one of the possible types of shaders that forms a GpuProgram.
	* Can be compiled independently and used on various GpuPrograms.
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgramStage : public Resource
	{
	public:
		GpuProgramStage(const std::string &filePath, GpuProgramStageType type);

		void addUniformDescription(const std::string &name, GpuProgramUniformSemantic semantic,
			GpuProgramUniformSize size = LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN, 
			GpuProgramUniformType type = LAG_GPU_PROG_UNIFORM_TYPE_UNKNOWN);
		
		inline GpuProgramStageType getType() const { return type; }
		inline uint32 getUniformDescriptionCount() const { return uniformDescriptions.size(); }
		inline const GpuProgramUniformDescription& getUniformDescription(int i) const { return uniformDescriptions[i]; }

		virtual bool compile() = 0;

	protected:
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		GpuProgramStageType type;
		std::string code;

		//Details of the Uniforms declared for this stage
		std::vector<GpuProgramUniformDescription> uniformDescriptions;
	};
}