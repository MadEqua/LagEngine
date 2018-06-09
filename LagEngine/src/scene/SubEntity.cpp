#include "SubEntity.h"

#include "RenderQueue.h"
#include "Renderer.h"
#include "SubMesh.h"
#include "VertexData.h"
#include "Material.h"
#include "GpuProgram.h"
#include "Entity.h"
#include "SceneNode.h"
#include "MaterialManager.h"

using namespace Lag;

SubEntity::SubEntity(Entity &parent, Material &material, SubMesh &subMesh) :
        material(material),
        subMesh(subMesh),
        parent(parent) {
}

void SubEntity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    RenderOperation &ro = renderQueue.addRenderOperation();
    ro.renderMode = parent.getRenderMode();
    ro.renderTarget = &renderTarget;
    ro.vertexData = const_cast<VertexData *>(&subMesh.getVertexData());
    ro.indexData = const_cast<IndexData *>(&subMesh.getIndexData());
    ro.renderable = this;
    ro.viewport = &viewport;
    ro.passId = 0;


    if (renderTarget.getRenderPhase() == RenderPhase::DEPTH) {
        //TODO out of here
        Material *depthPassMaterial = Root::getInstance().getMaterialManager().get("depthPassMaterial").get();

        ro.material = depthPassMaterial;
    }
    else {
        ro.material = &material;
    }
}

void SubEntity::render(Renderer &renderer, RenderOperation &renderOperation) {
    renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
                                                   parent.getTransform(), parent.getNormalTransform(),
                                                   *renderOperation.viewport);

    renderer.renderIndexed(renderOperation.renderMode, *renderOperation.vertexData, *renderOperation.indexData,
                           renderOperation.vertexData->vertexStart);
}