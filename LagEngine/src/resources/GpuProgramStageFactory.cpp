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
	if (gpuProgramStageType == "vertex") this->gpuProgramStageType = LAG_GPU_PROG_STAGE_TYPE_VERTEX;
	else if (gpuProgramStageType == "tesselationControl") this->gpuProgramStageType = GpuProgramStageType::LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL;
	else if (gpuProgramStageType == "tesselationEvaluation") this->gpuProgramStageType = GpuProgramStageType::LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION;
	else if (gpuProgramStageType == "geometry") this->gpuProgramStageType = LAG_GPU_PROG_STAGE_TYPE_GEOMETRY;
	else if (gpuProgramStageType == "fragment") this->gpuProgramStageType = LAG_GPU_PROG_STAGE_TYPE_FRAGMENT;
}

GpuProgramStageFactory::~GpuProgramStageFactory()
{
}

Resource* GpuProgramStageFactory::create() const
{
	if (graphicsApiType == LAG_GRAPHICS_API_TYPE_OPENGL_4)
	{
		return new GL4GpuProgramStage(path, gpuProgramStageType);
	}
	return nullptr;
}