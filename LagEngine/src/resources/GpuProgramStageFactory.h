#pragma once

#include "IResourceFactory.h"
#include <string>

namespace Lag
{
	enum GpuProgramStageType;

	/*
	* To be overriden by each supported GraphicsAPI.
	*/
	class GpuProgramStageFactory : public IResourceFactory
	{
	public:
		GpuProgramStageFactory() {}
		virtual ~GpuProgramStageFactory() {}

		virtual Resource* create() const = 0;

		std::string path;
		GpuProgramStageType type;
	};
}