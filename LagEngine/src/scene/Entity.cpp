#include "Entity.h"

#include "Root.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "Renderer.h"
#include "SubEntity.h"
#include "Mesh.h"

using namespace Lag;

Entity::Entity(const std::string &meshName, const std::string &materialName) {
    setMesh(meshName);
    setMaterial(materialName);
}

Entity::Entity(Handle<Mesh> meshHandle, const std::string &materialName) {
    setMesh(meshHandle);
    setMaterial(materialName);
}

Entity::Entity() : Entity("defaultMesh", "defaultMaterial") {
}

void Entity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    for (auto &ptr : subEntities)
        ptr->addToRenderQueue(renderQueue, viewport, renderTarget);

#ifdef ENABLE_AABB_GIZMOS
    if(renderTarget.getRenderPhase() == RenderPhase::COLOR) {
        RenderOperation &ro = renderQueue.addRenderOperation();
        ro.renderTarget = &renderTarget;
        ro.vertexData = const_cast<VertexData *>(aabbMesh->getSubMeshes()[0]->getVertexData());
        ro.indexData = const_cast<IndexData *>(aabbMesh->getSubMeshes()[0]->getIndexData());
        ro.renderable = this;
        ro.viewport = &viewport;
        ro.passId = 0;
        ro.material = aabbMaterial.get();
    }
#endif
}

void Entity::render(Renderer &renderer, RenderOperation &renderOperation) {
    for (auto &ptr : subEntities)
        ptr->render(renderer, renderOperation);

#ifdef ENABLE_AABB_GIZMOS
    renderer.renderIndexed(renderOperation.material->getRenderMode(), *renderOperation.vertexData, *renderOperation.indexData);
#endif
}

void Entity::setMaterial(const std::string &materialName) {
    setMaterial(Root::getInstance().getMaterialManager().get(materialName));
}

void Entity::setMaterial(Handle<Material> material) {
    for (auto &subEntity : subEntities) {
        subEntity->setMaterial(*material);
    }

    this->material = material;

#ifdef ENABLE_AABB_GIZMOS
    aabbMaterial = Root::getInstance().getMaterialManager().get("aabbGizmoMaterial");
#endif
}

void Entity::setMesh(const std::string &meshName) {
    setMesh(Root::getInstance().getMeshManager().get(meshName));
}

void Entity::setMesh(Handle<Mesh> mesh) {
    subEntities.clear();
    for (auto &subMeshPtr : mesh->getSubMeshes()) {
        auto se = new SubEntity(*this);
        se->setSubMesh(*subMeshPtr);
        if(material.isValid())
            se->setMaterial(*material);
        subEntities.push_back(std::unique_ptr<SubEntity>(se));
    };

    this->mesh = mesh;

#ifdef ENABLE_AABB_GIZMOS
    aabbMesh = Root::getInstance().getMeshManager().getAABBGizmo();
#endif
}