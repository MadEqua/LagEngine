#pragma once

#include <vector>

#include "../Types.h"

namespace Lag
{
	class GpuProgramUniform;
	class GpuProgramStage;
	class GpuProgram;
	
	/*
	* Represents and manages a full set of uniforms in use on a GpuProgram.
	* Manages a contiguous block of memory that stores (tightly) the values of all the Uniforms of a GpuProgram.
	* Each Uniform will have a pointer to its space in that memory.
	*/
	class GpuProgramUniforms
	{
	public:
		GpuProgramUniforms(const GpuProgram &program, const std::vector<GpuProgramStage*> stages);
		virtual ~GpuProgramUniforms();

		void sendToGpu() const;

	private:
		const GpuProgram &program;
		std::vector<GpuProgramUniform*> uniforms;
		std::vector<byte> dataPool;
	};
}