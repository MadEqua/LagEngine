#pragma once

#include <string>
#include "../Types.h"

namespace Lag
{
	enum GpuProgramUniformSize
	{
		LAG_GPU_PROG_UNIFORM_SIZE_1,
		LAG_GPU_PROG_UNIFORM_SIZE_2,
		LAG_GPU_PROG_UNIFORM_SIZE_3,
		LAG_GPU_PROG_UNIFORM_SIZE_4
	};

	enum GpuProgramUniformType
	{
		LAG_GPU_PROG_UNIFORM_TYPE_BOOL,
		LAG_GPU_PROG_UNIFORM_TYPE_FLOAT,
		LAG_GPU_PROG_UNIFORM_TYPE_INT32,
		LAG_GPU_PROG_UNIFORM_TYPE_UINT32,
		LAG_GPU_PROG_UNIFORM_TYPE_SAMPLER,
		LAG_GPU_PROG_UNIFORM_TYPE_MATRIX
	};

	enum GpuProgramUniformSemantic
	{
		LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX,
		LAG_GPU_PROG_UNI_SEM_MODELVIEW_MATRIX,
		LAG_GPU_PROG_UNI_SEM_MVP_MATRIX,
		LAG_GPU_PROG_UNI_SEM_CUSTOM
	};
	
	/*
	* Container for all the data that describe an Uniform. Belongs to a GpuProgramStage.
	*/
	struct GpuProgramUniformDescription
	{
		std::string name; //needed?
		GpuProgramUniformSize size;
		GpuProgramUniformType type;
		GpuProgramUniformSemantic semantic;

		uint8 getSize() const;
		uint8 getSizeBytes() const;
	};
}