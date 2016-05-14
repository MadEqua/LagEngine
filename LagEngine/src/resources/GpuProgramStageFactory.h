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
		GpuProgramStageFactory(const std::string &path, GpuProgramStageType gpuProgramStageType, GraphicsApiType graphicsApiType);
		GpuProgramStageFactory(const std::string &path,	const std::string &gpuProgramStageType, GraphicsApiType graphicsApiType);
		
		virtual ~GpuProgramStageFactory();

		virtual Resource* create() const override;

	private:
		std::string path;
		GpuProgramStageType gpuProgramStageType;
		GraphicsApiType graphicsApiType;
	};
}