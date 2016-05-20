#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class GpuProgramStageFactory;
	enum GpuProgramStageType;
	
	class GpuProgramStageManager : public ResourceManager
	{
	public:
		GpuProgramStageManager(GpuProgramStageFactory *factory);
		~GpuProgramStageManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
		static GpuProgramStageType parseTypeFromString(const std::string &type);
	};
}