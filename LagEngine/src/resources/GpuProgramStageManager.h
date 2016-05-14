#pragma once

#include "ResourceManager.h"

namespace Lag
{
	enum GraphicsApiType;
	
	class GpuProgramStageManager : public ResourceManager
	{
	public:
		GpuProgramStageManager();
		~GpuProgramStageManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}