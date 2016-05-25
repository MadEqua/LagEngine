#include "SubEntity.h"

#include "../renderer/RenderQueue.h"
#include "../renderer/Renderer.h"
#include "../renderer/SubMesh.h"
#include "../renderer/VertexData.h"
#include "../renderer/Material.h"

using namespace Lag;

SubEntity::SubEntity(Material &material, SubMesh &subMesh) :
	material(material),
	subMesh(subMesh)
{
}

SubEntity::~SubEntity()
{
}

void SubEntity::addToRenderQueue(RenderQueue &renderQueue)
{
	renderQueue.addRenderOperation(*this, 0,
		const_cast<VertexData&>(subMesh.getVertexData()), 
		const_cast<IndexData*>(&subMesh.getIndexData()),
		material);
}

void SubEntity::render(Renderer &renderer, uint32 passId)
{
	renderer.bindGpuProgram(material.getGpuProgram());
	renderer.renderIndexed(subMesh.getVertexData(), subMesh.getIndexData(), subMesh.getVertexData().vertexStart);
}