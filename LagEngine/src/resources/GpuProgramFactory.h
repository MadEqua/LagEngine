#pragma once

#include <string>
#include <vector>

#include "IResourceFactory.h"

namespace Lag
{
	class GpuProgramStage;
	enum GraphicsApiType;
	
	class GpuProgramFactory : public IResourceFactory
	{
	public:
		GpuProgramFactory(const std::vector<GpuProgramStage*> &stages, GraphicsApiType graphicsApiType);
		GpuProgramFactory(const std::vector<std::string> &gpuProgramStageNames, GraphicsApiType graphicsApiType);
		virtual ~GpuProgramFactory();

		virtual Resource* create() const override;

	private:
		std::vector<GpuProgramStage*> stages;
		GraphicsApiType graphicsApiType;
	};
}