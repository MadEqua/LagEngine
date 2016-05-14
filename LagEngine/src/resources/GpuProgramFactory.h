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
		GpuProgramFactory();
		virtual ~GpuProgramFactory();

		void addGpuProgramStage(const std::string &name);
		void addGpuProgramStage(GpuProgramStage& stage);
		void setGraphicsApiType(GraphicsApiType graphicsApiType);

		virtual Resource* create() const override;

	private:
		std::vector<GpuProgramStage*> stages;
		GraphicsApiType graphicsApiType;
	};
}