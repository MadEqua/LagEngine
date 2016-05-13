#pragma once

#include <string>
#include "ResourceManager.h"
#include "../renderer/GpuInterfaceType.h"

namespace Lag
{
	class GpuProgramStageManager : public ResourceManager
	{
	public:
		GpuProgramStageManager(const GpuInterfaceType gpuInterfaceType);
		~GpuProgramStageManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;

		GpuInterfaceType gpuInterfaceType;
	};
}