#include "SubEntity.h"

#include "RenderQueue.h"
#include "Renderer.h"
#include "SubMesh.h"
#include "Material.h"
#include "GpuProgram.h"
#include "Entity.h"
#include "SceneNode.h"
#include "MaterialManager.h"

using namespace Lag;

SubEntity::SubEntity(Entity &parent, Material &material, SubMesh &subMesh) :
        material(&material),
        subMesh(&subMesh),
        parent(parent) {
}

SubEntity::SubEntity(Entity &parent) :
        material(nullptr),
        subMesh(nullptr),
        parent(parent) {
}

void SubEntity::setMaterial(Material &material) {
    this->material = &material;
}

void SubEntity::setSubMesh(SubMesh &subMesh) {
    this->subMesh = &subMesh;
}

void SubEntity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    RenderOperation &ro = renderQueue.addRenderOperation();
    ro.renderTarget = &renderTarget;
    ro.vertexData = const_cast<VertexData *>(subMesh->getVertexData());
    ro.indexData = const_cast<IndexData *>(subMesh->getIndexData());
    ro.renderable = this;
    ro.viewport = &viewport;
    ro.passId = 0;

    if (renderTarget.getRenderPhase() == RenderPhase::DEPTH) {
        //TODO get this out of here
        Material *depthPassMaterial = Root::getInstance().getMaterialManager().get("depthPassMaterial").get();
        ro.material = depthPassMaterial;
    }
    else {
        ro.material = material;
    }
}

void SubEntity::preRender(Renderer &renderer, const RenderOperation &renderOperation) {
    renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
                                                   *renderOperation.viewport,
                                                   parent.getLocalToWorldTransform(),
                                                   parent.getNormalTransform());

    parent.onSubEntityPreRender(*this, renderer, renderOperation);
}


void SubEntity::render(Renderer &renderer, const RenderOperation &renderOperation) {
    RenderMode renderMode = renderOperation.material->getRenderMode();
    if(renderMode == RenderMode::POINTS) {
        renderer.renderVertices(renderMode, *renderOperation.vertexData);
    }
    else {
        renderer.renderIndexed(renderMode, *renderOperation.vertexData, *renderOperation.indexData);
    }

    parent.onSubEntityRender(*this, renderer, renderOperation);
}
