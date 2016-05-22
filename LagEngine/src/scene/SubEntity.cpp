#include "SubEntity.h"

#include "../renderer/RenderQueue.h"
#include "../renderer/RenderOperation.h"
#include "../renderer/SubMesh.h"
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
	renderQueue.addItem(LAG_RENDER_COMM_MULTI_INDEXED, LAG_RENDER_TYPE_TRIANGLES,
		const_cast<VertexData&>(subMesh.getVertexData()), 
		const_cast<IndexData&>(subMesh.getIndexData()),
		material.getShaderProgram());
}