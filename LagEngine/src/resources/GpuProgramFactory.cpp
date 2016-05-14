#include "GpuProgramFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgram.h"
#include "../renderer/GraphicsApiType.h"
#include "GpuProgramStageManager.h"
#include "../Root.h"

using namespace Lag;

GpuProgramFactory::GpuProgramFactory(const std::vector<GpuProgramStage*> &stages, GraphicsApiType graphicsApiType) : 
	stages(stages),
	graphicsApiType(graphicsApiType)
{
}

GpuProgramFactory::GpuProgramFactory(const std::vector<std::string> &gpuProgramStageNames, GraphicsApiType graphicsApiType) :
	graphicsApiType(graphicsApiType)
{
	for (const std::string &name : gpuProgramStageNames)
	{
		Resource *stage = Root::getInstance().getGpuProgramStageManager().get(name);
		stages.push_back((GpuProgramStage*)stage);
	}
}

GpuProgramFactory::~GpuProgramFactory()
{
}

Resource* GpuProgramFactory::create() const
{
	if (graphicsApiType == OPENGL_4)
	{
		return new GL4GpuProgram(stages);
	}
	return nullptr;
}