#include "GpuProgramStageFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgramStage.h"
#include "../renderer/GraphicsApiType.h"
#include "../renderer/graphicsAPI/GpuProgramStageType.h"

using namespace Lag;

GpuProgramStageFactory::GpuProgramStageFactory(const std::string &path, GpuProgramStageType gpuProgramStageType, GraphicsApiType graphicsApiType) :
	path(path),
	gpuProgramStageType(gpuProgramStageType),
	graphicsApiType(graphicsApiType)
{
}

GpuProgramStageFactory::GpuProgramStageFactory(const std::string &path, const std::string &gpuProgramStageType, GraphicsApiType graphicsApiType) :
	path(path),
	graphicsApiType(graphicsApiType)
{
	if (gpuProgramStageType == "vertex") this->gpuProgramStageType = VERTEX;
	else if (gpuProgramStageType == "tesselationControl") this->gpuProgramStageType = TESSELLATION_CONTROL;
	else if (gpuProgramStageType == "tesselationEvaluation") this->gpuProgramStageType = TESSELLATION_EVALUATION;
	else if (gpuProgramStageType == "geometry") this->gpuProgramStageType = GEOMETRY;
	else if (gpuProgramStageType == "fragment") this->gpuProgramStageType = FRAGMENT;
}

GpuProgramStageFactory::~GpuProgramStageFactory()
{
}

Resource* GpuProgramStageFactory::create() const
{
	if (graphicsApiType == OPENGL_4)
	{
		return new GL4GpuProgramStage(path, gpuProgramStageType);
	}
	return nullptr;
}