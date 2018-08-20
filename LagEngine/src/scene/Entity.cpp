#include "Entity.h"

#include "Renderer.h"
#include "SubEntity.h"
#include "Mesh.h"

using namespace Lag;

Entity::Entity(Handle<Material> defaultMaterial, Handle<Mesh> mesh) :
        defaultMaterial(defaultMaterial),
        mesh(mesh) {
    for (auto &subMeshPtr : mesh->getSubMeshes()) {
        auto *se = new SubEntity(*this, *defaultMaterial, *subMeshPtr);
        subEntities.push_back(se);
    }
}

Entity::~Entity() {
    for (SubEntity *se : subEntities)
        delete se;
}

void Entity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    for (SubEntity *se : subEntities)
        se->addToRenderQueue(renderQueue, viewport, renderTarget);
}

void Entity::render(Renderer &renderer, RenderOperation &renderOperation) {
    for (SubEntity *se : subEntities)
        se->render(renderer, renderOperation);
}