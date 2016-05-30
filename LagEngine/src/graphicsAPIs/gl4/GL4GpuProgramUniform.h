#pragma once

#include "../../renderer/graphicsAPI/GpuProgramUniform.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgramUniform : public GpuProgramUniform
	{
	public:
		GL4GpuProgramUniform(const GpuProgramUniformDescription &description, const GpuProgram &gpuProgram);
		~GL4GpuProgramUniform();

		virtual void setValue(const void* value, uint32 arraySize = 1) const override;
	
	protected:
		GLint location;
	};
}