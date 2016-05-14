#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class GpuProgramManager : public ResourceManager
	{

	public:
		GpuProgramManager();
		virtual ~GpuProgramManager();

	protected:

		/*
		 * Does nothing. GpuPrograms are not defined on Resources files.
		 * They are created as needed from GpuProgramStages combinations defined on Material files.
		 */
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}