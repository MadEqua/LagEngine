#pragma once

#include <string>
#include "../../Types.h"

namespace Lag
{
	struct GpuProgramUniformDescription;
	class GpuProgram;
	
	/*
	* Represents an Uniform instance i.e. in use by a GpuProgram.
	* Each GraphicsAPI will have a concrete implementation.
	*/
	class GpuProgramUniform
	{
	public:
		GpuProgramUniform(const GpuProgramUniformDescription &description, const GpuProgram &gpuProgram);
		virtual ~GpuProgramUniform();

		//The implementation can deduce what to send based on the GpuProgramUniformDescription
		virtual void setValue(const void* value) const = 0;

		const GpuProgramUniformDescription& getGpuProgramUniformDescription() const { return description; }

	protected:
		const GpuProgramUniformDescription &description;
	};
}