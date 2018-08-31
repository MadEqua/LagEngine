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
}

void Entity::render(Renderer &renderer, RenderOperation &renderOperation) {
    for (auto &ptr : subEntities)
        ptr->render(renderer, renderOperation);
}

void Entity::setMaterial(const std::string &materialName) {
    setMaterial(Root::getInstance().getMaterialManager().get(materialName));
}

void Entity::setMaterial(Handle<Material> material) {
    for (auto &subEntity : subEntities) {
        subEntity->setMaterial(*material);
    }

    this->material = material;
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
}