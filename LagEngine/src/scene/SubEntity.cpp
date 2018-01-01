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
#include "../Root.h"
#include "../resources/MaterialManager.h"
#include "../renderer/RenderTarget.h"

using namespace Lag;

SubEntity::SubEntity(Entity &parent, Material &material, SubMesh &subMesh) :
	material(material),
	subMesh(subMesh),
	parent(parent)
{
}

void SubEntity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget)
{
	RenderOperation &ro = renderQueue.addRenderOperation();
	ro.renderTarget = &renderTarget;
	ro.vertexData = const_cast<VertexData*>(&subMesh.getVertexData());
	ro.indexData = const_cast<IndexData*>(&subMesh.getIndexData());
	ro.renderable = this;
	ro.viewport = &viewport;
	ro.passId = 0;
	
	
	if (renderTarget.getRenderPhase() == LAG_RENDER_PHASE_DEPTH)
	{
		//TODO out of here
		Material *depthPassMaterial =
			static_cast<Material*>(Root::getInstance().getMaterialManager().get("depthPassMaterial"));

		ro.material = depthPassMaterial;
	}
	else
	{
		ro.material = &material;
	}
}

void SubEntity::render(Renderer &renderer, RenderOperation &renderOperation)
{
	renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
		parent.getTransform(), parent.getNormalTransform(), *renderOperation.viewport);

	renderer.renderIndexed(*renderOperation.vertexData, *renderOperation.indexData,
		renderOperation.vertexData->vertexStart);
}