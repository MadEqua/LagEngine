#include "SubEntity.h"

#include "../renderer/RenderQueue.h"
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
	renderQueue.addRenderOperation(*this, 0,
		const_cast<VertexData&>(subMesh.getVertexData()), 
		const_cast<IndexData*>(&subMesh.getIndexData()),
		material);
}

void SubEntity::render(IGraphicsAPI &graphicsAPI, uint32 passId)
{
	//TODO
}