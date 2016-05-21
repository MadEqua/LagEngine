#pragma once

#include <string>
#include "../../Types.h"

namespace Lag
{
	struct GpuProgramUniformDescription;	
	
	/*
	* Represents an Uniform instance i.e. in use by a GpuProgram.
	*
	* Its value is stored in a memory location managed by GpuProgramUniforms to 
	* avoid each Uniform having to allocate its own memory, leading to lots of fragmentation.
	* 
	* Each GraphicsAPI will have a concrete implementation.
	*/
	class GpuProgramUniform
	{
	public:
		GpuProgramUniform(const GpuProgramUniformDescription &description, void* dataLocation);
		virtual ~GpuProgramUniform();

		template<class T>
		void setValue(T value);

		template<class T>
		void setValue(T *value);

		//TODO: sampler?

		void sendToGpu();

		const GpuProgramUniformDescription& getGpuProgramUniformDescription() const { return description; }

	protected:
		virtual void sendToGpuImpl() const = 0;

		void internalSetValue(void* value);

		const GpuProgramUniformDescription &description;

		//Pointing to memory managed by GpuProgramUniforms
		void* dataPtr;
		
		//Changed but not sent to Gpu
		bool isDirty; 
	};
}