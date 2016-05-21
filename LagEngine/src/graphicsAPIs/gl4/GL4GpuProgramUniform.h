#pragma once

#include "../../renderer/graphicsAPI/GpuProgramUniform.h"

namespace Lag
{
	class GL4GpuProgramUniform : public GpuProgramUniform
	{
	public:
		GL4GpuProgramUniform(const GpuProgramUniformDescription &description, void* dataLocation);
		~GL4GpuProgramUniform();

	protected:
		virtual void sendToGpuImpl() const override;
	};
}