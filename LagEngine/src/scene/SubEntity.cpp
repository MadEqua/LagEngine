#include "SubEntity.h"

#include "../renderer/RenderQueue.h"
#include "../renderer/Renderer.h"
#include "../renderer/SubMesh.h"
#include "../renderer/VertexData.h"
#include "../renderer/Material.h"
#include "../renderer/graphicsAPI/GpuProgram.h"
#include "../renderer/graphicsAPI/GpuProgramUniform.h"
#include "../renderer/GpuProgramUniformDescription.h"
#include "Entity.h"
#include "../scene/SceneNode.h"
#include "../renderer/Viewport.h"
#include "Camera.h"

using namespace Lag;

SubEntity::SubEntity(Entity &parent, Material &material, SubMesh &subMesh) :
	material(material),
	subMesh(subMesh),
	parent(parent)
{
}

SubEntity::~SubEntity()
{
}

void SubEntity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport)
{
	renderQueue.addRenderOperation(*this, 0,
		const_cast<VertexData&>(subMesh.getVertexData()), 
		const_cast<IndexData*>(&subMesh.getIndexData()),
		material, viewport);
}

void SubEntity::render(Renderer &renderer, RenderOperation &renderOperation)
{
	const GpuProgram &gpuProgram = material.getGpuProgram();
	auto uniformList = gpuProgram.getUniformBySemantic(LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX);
	if (uniformList != nullptr)
	{
		GpuProgramUniform *modelMatrixUniform = uniformList->at(0);
		glm::mat4 model = this->parent.getTransform();
		modelMatrixUniform->setValue(&model);
	}

	uniformList = gpuProgram.getUniformBySemantic(LAG_GPU_PROG_UNI_SEM_VIEWPROJECTION_MATRIX);
	if (uniformList != nullptr)
	{
		GpuProgramUniform *vpMatrixUniform = uniformList->at(0);
		const Camera &camera = renderOperation.viewport->getCamera();
		glm::mat4 vp = camera.getProjectionMatrix() *
			camera.getInverseTransform();

		vpMatrixUniform->setValue(&vp);
	}

	//TODO: take out the normal calculation from here
	uniformList = gpuProgram.getUniformBySemantic(LAG_GPU_PROG_UNI_SEM_NORMAL_MATRIX);
	if (uniformList != nullptr)
	{
		GpuProgramUniform *normalMatrixUniform = uniformList->at(0);
		glm::mat3 nor = glm::transpose(glm::mat3(parent.getInverseTransform()));
		normalMatrixUniform->setValue(&nor);
	}
	
	renderer.bindGpuProgram(gpuProgram);
	renderer.renderIndexed(subMesh.getVertexData(), subMesh.getIndexData(), subMesh.getVertexData().vertexStart);
}