#pragma once

#include "ResourceManager.h"

namespace Lag
{
	enum GpuProgramStageType;
	
	class GpuProgramStageManager : public ResourceManager
	{
	public:
		GpuProgramStageManager();
		~GpuProgramStageManager();

		virtual void create(const std::string &name, const std::string &file, GpuProgramStageType type) = 0;

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
		static GpuProgramStageType parseTypeFromString(const std::string &type);
	};
}