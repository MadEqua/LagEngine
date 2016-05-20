#pragma once

#include "../../renderer/graphicsAPI/GpuProgramUniform.h"

namespace Lag
{
	class GL4GpuProgramUniform : public GpuProgramUniform
	{
	public:
		GL4GpuProgramUniform(uint32 location, GpuProgramUniformSize size,
			GpuProgramUniformType type, GpuProgramUniformSemantic semantic,
			void* dataLocation);
		~GL4GpuProgramUniform();

	protected:
		virtual void sendToGpuImpl() const override;
	};
}