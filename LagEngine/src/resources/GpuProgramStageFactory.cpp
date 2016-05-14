#include "GpuProgramStageFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgramStage.h"
#include "../renderer/GraphicsApiType.h"
#include "../renderer/graphicsAPI/GpuProgramStageType.h"

using namespace Lag;

GpuProgramStageFactory::GpuProgramStageFactory()
{
}

GpuProgramStageFactory::~GpuProgramStageFactory()
{
}

void GpuProgramStageFactory::setPath(const std::string &path)
{
	this->path = path;
}

void GpuProgramStageFactory::setGpuProgramStageType(GpuProgramStageType gpuProgramStageType)
{
	this->gpuProgramStageType = gpuProgramStageType;
}

void GpuProgramStageFactory::setGpuProgramStageType(const std::string &gpuProgramStageType)
{
	if (gpuProgramStageType == "vertex") this->gpuProgramStageType = VERTEX;
	else if (gpuProgramStageType == "tesselationControl") this->gpuProgramStageType = TESSELLATION_CONTROL;
	else if (gpuProgramStageType == "tesselationEvaluation") this->gpuProgramStageType = TESSELLATION_EVALUATION;
	else if (gpuProgramStageType == "geometry") this->gpuProgramStageType = GEOMETRY;
	else if (gpuProgramStageType == "fragment") this->gpuProgramStageType = FRAGMENT;
}

void GpuProgramStageFactory::setGraphicsApiType(GraphicsApiType graphicsApiType)
{
	this->graphicsApiType = graphicsApiType;
}

Resource* GpuProgramStageFactory::create() const
{
	if (graphicsApiType == OPENGL_4)
	{
		return new GL4GpuProgramStage(path, gpuProgramStageType);
	}
	return nullptr;
}