#include "GpuProgramFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgram.h"
#include "../renderer/GraphicsApiType.h"

using namespace Lag;

GpuProgramFactory::GpuProgramFactory(const std::vector<GpuProgramStage*> &stages, GraphicsApiType graphicsApiType) : 
	stages(stages),
	graphicsApiType(graphicsApiType)
{
}

GpuProgramFactory::GpuProgramFactory(const std::vector<std::string> &gpuProgramStageNames, GraphicsApiType graphicsApiType) :
	stagesNames(gpuProgramStageNames),
	graphicsApiType(graphicsApiType)
{
}

GpuProgramFactory::~GpuProgramFactory()
{
}

Resource* GpuProgramFactory::create() const
{
	if (graphicsApiType == OPENGL_4)
	{
		if (!stages.empty())
			return new GL4GpuProgram(stages);
		else
			return new GL4GpuProgram(stagesNames);
	}
	return nullptr;
}