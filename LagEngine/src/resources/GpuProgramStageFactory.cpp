#include "GpuProgramStageFactory.h"

#include "../graphicsAPIs/gl4/GL4GpuProgramStage.h"
#include "../renderer/GraphicsApiType.h"

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
	else if (gpuProgramStageType == "tesselationControl") this->gpuProgramStageType = GpuProgramStageType::TESS_CONTROL;
	else if (gpuProgramStageType == "tesselationEvaluation") this->gpuProgramStageType = GpuProgramStageType::TESS_EVALUATION;
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