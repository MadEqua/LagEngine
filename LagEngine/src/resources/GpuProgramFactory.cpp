#include "GpuProgramFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgram.h"
#include "../renderer/GraphicsApiType.h"
#include "GpuProgramStageManager.h"
#include "../Root.h"

using namespace Lag;

GpuProgramFactory::GpuProgramFactory() 
{
}

GpuProgramFactory::~GpuProgramFactory()
{
}

void GpuProgramFactory::addGpuProgramStage(const std::string &name)
{
	Resource *stage = Root::getInstance().getGpuProgramStageManager().get(name);
	addGpuProgramStage(*((GpuProgramStage*)stage));
}
void GpuProgramFactory::addGpuProgramStage(GpuProgramStage& stage)
{
	stages.push_back(&stage);
}

void GpuProgramFactory::setGraphicsApiType(GraphicsApiType graphicsApiType)
{
	this->graphicsApiType = graphicsApiType;
}

Resource* GpuProgramFactory::create() const
{
	if (graphicsApiType == OPENGL_4)
	{
		return new GL4GpuProgram(stages);
	}
	return nullptr;
}