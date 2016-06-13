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
#include "../renderer/graphicsAPI/Texture.h"

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
	//TODO: if castShadow?, correct material...
	renderQueue.addRenderOperation(*this, LAG_RENDER_PHASE_DEPTH, 0,
		const_cast<VertexData&>(subMesh.getVertexData()),
		const_cast<IndexData*>(&subMesh.getIndexData()),
		material, viewport);
	
	
	renderQueue.addRenderOperation(*this, LAG_RENDER_PHASE_OPAQUE, 0,
		const_cast<VertexData&>(subMesh.getVertexData()), 
		const_cast<IndexData*>(&subMesh.getIndexData()),
		material, viewport);
}

void SubEntity::render(Renderer &renderer, RenderOperation &renderOperation)
{
	renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
		parent.getTransform(),
		parent.getNormalTransform(),
		*renderOperation.viewport);

	renderer.renderIndexed(subMesh.getVertexData(), subMesh.getIndexData(), subMesh.getVertexData().vertexStart);
}