#include "GpuProgramUniformFiller.h"

#include "../Root.h"
#include "../scene/SceneManager.h"
#include "../scene/PointLight.h"
#include "../scene/DirectionalLight.h"
#include "../scene/Camera.h"

#include "GpuProgramUniformDescription.h"
#include "graphicsAPI/GpuProgram.h"
#include "graphicsAPI/GpuProgramUniform.h"
#include "Viewport.h"

#include <glm/vec3.hpp>
#include <algorithm>

using namespace Lag;

GpuProgramUniformFiller::GpuProgramUniformFiller() :
	pointLights(Root::getInstance().getSceneManager().getPointLights()),
	directionalLights(Root::getInstance().getSceneManager().getDirectionalLights())
{
}

GpuProgramUniformFiller::~GpuProgramUniformFiller()
{
}

GpuProgramUniformFiller::GpuProgramData& GpuProgramUniformFiller::getGpuProgramData(const GpuProgram* gpuProgram)
{
	comparator.program = gpuProgram;
	auto it = std::find_if(gpuPrograms.begin(), gpuPrograms.end(), comparator);
	
	if (gpuPrograms.empty() || it == gpuPrograms.end())
	{
		gpuPrograms.push_back(std::make_pair(gpuProgram, GpuProgramData()));
		return gpuPrograms[gpuPrograms.size() - 1].second;
	}
	else
	{
		return it->second;
	}
}

void GpuProgramUniformFiller::onFrameStart(const GpuProgram *gpuProgram, uint64 frameNumber, const Viewport *viewport)
{
	if (!gpuProgram || !viewport) return;
	updateLightUniforms(*gpuProgram);
	updateViewportUniforms(*gpuProgram, *viewport);
}

void GpuProgramUniformFiller::onGpuProgramBind(const GpuProgram *gpuProgram, uint64 frameNumber, const Viewport *viewport)
{
	if (!gpuProgram || !viewport) return;
	
	GpuProgramData &data = getGpuProgramData(gpuProgram);

	if (data.lastUpdatedFrame < frameNumber || frameNumber == 0)
	{
		updateLightUniforms(*gpuProgram);
		data.lastUpdatedFrame = frameNumber;
	}
	if (data.lastUpdatedViewport != viewport)
	{
		updateViewportUniforms(*gpuProgram, *viewport);
		data.lastUpdatedViewport = viewport;
	}
}

void GpuProgramUniformFiller::onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport)
{
	if (!gpuProgram || !viewport) return;
	
	GpuProgramData &data = getGpuProgramData(gpuProgram);
	
	if (data.lastUpdatedViewport != viewport)
	{
		updateViewportUniforms(*gpuProgram, *viewport);
		data.lastUpdatedViewport = viewport;
	}
}

void GpuProgramUniformFiller::updateLightUniforms(const GpuProgram &gpuProgram)
{
	//PointLights
	uint32 pointLightCount = pointLights.size();
	setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COUNT, &pointLightCount);

	if (pointLightCount > 0)
	{
		glm::vec3 ppos[Renderer::MAX_POINT_LIGHTS];
		glm::vec3 pcols[Renderer::MAX_POINT_LIGHTS];
		for (uint32 i = 0; i < Renderer::MAX_POINT_LIGHTS && i < pointLightCount; ++i)
		{
			ppos[i] = pointLights[i]->getWorldPosition();
			const float *floatColors = pointLights[i]->getColor().getRGBAfloat();
			pcols[i].r = floatColors[0];
			pcols[i].g = floatColors[1];
			pcols[i].b = floatColors[2];
		}
		setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_POSITIONS, ppos, pointLightCount);
		setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COLORS, pcols, pointLightCount);
	}

	//DirectionalLights
	uint32 dirLightCount = directionalLights.size();
	setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COUNT, &dirLightCount);

	if (dirLightCount > 0)
	{
		glm::vec3 ddir[Renderer::MAX_DIRECTIONAL_LIGHTS];
		glm::vec3 dcols[Renderer::MAX_DIRECTIONAL_LIGHTS];
		for (uint32 i = 0; i < Renderer::MAX_DIRECTIONAL_LIGHTS && i < dirLightCount; ++i)
		{
			ddir[i] = directionalLights[i]->getDirection();
			const float *floatColors = directionalLights[i]->getColor().getRGBAfloat();
			dcols[i].r = floatColors[0];
			dcols[i].g = floatColors[1];
			dcols[i].b = floatColors[2];
		}
		setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_DIRECTIONS, ddir, dirLightCount);
		setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COLORS, dcols, dirLightCount);
	}
}

void GpuProgramUniformFiller::updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport)
{
	//Camera-only related matrices
	const glm::mat4 &view = viewport.getCamera().getInverseTransform();
	const glm::mat4 &proj = viewport.getCamera().getProjectionMatrix();

	setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_VIEW_MATRIX, static_cast<const void*>(&view));
	setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_PROJECTION_MATRIX, static_cast<const void*>(&proj));

	if (programContainsUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_VIEWPROJECTION_MATRIX))
	{
		glm::mat4 viewProj = proj * view;
		setUniform(gpuProgram, LAG_GPU_PROG_UNI_SEM_VIEWPROJECTION_MATRIX, &viewProj);
	}
}

bool GpuProgramUniformFiller::programContainsUniform(const GpuProgram &gpuProgram,
	GpuProgramUniformSemantic semantic)
{
	auto uniformList = gpuProgram.getUniformBySemantic(semantic);
	return uniformList != nullptr;
}

void GpuProgramUniformFiller::setUniform(const GpuProgram &gpuProgram, 
	GpuProgramUniformSemantic semantic, const void* value, uint32 arraySize)
{
	auto uniformList = gpuProgram.getUniformBySemantic(semantic);
	if (uniformList != nullptr)
		for (GpuProgramUniform *uniform : *uniformList)
			uniform->setValue(value, arraySize);
}