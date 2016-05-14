#pragma once

#include "IResourceFactory.h"
#include <string>

namespace Lag
{
	enum GpuProgramStageType;
	enum GraphicsApiType;
	
	class GpuProgramStageFactory : public IResourceFactory
	{
	public:
		GpuProgramStageFactory();
		virtual ~GpuProgramStageFactory();

		void setPath(const std::string &path);
		void setGpuProgramStageType(GpuProgramStageType gpuProgramStageType);
		void setGpuProgramStageType(const std::string &gpuProgramStageType);
		void setGraphicsApiType(GraphicsApiType graphicsApiType);

		virtual Resource* create() const override;

	private:
		std::string path;
		GpuProgramStageType gpuProgramStageType;
		GraphicsApiType graphicsApiType;
	};
}