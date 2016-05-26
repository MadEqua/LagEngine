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

		template<class T>
		void setValue(T value);

		template<class T>
		void setValue(T *value);


		const GpuProgramUniformDescription& getGpuProgramUniformDescription() const { return description; }

	protected:
		virtual void setValueImpl(void* value) const = 0;

		const GpuProgramUniformDescription &description;
	};
}