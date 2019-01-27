#include "Entity.h"

#include "Root.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "Renderer.h"
#include "SubEntity.h"
#include "Mesh.h"

#include "BoundingSphere.h"
#include "AABB.h"
#include "SceneNode.h"

using namespace Lag;

Entity::Entity(const std::string &meshName, const std::string &materialName) :
    collider(false) {
    setMesh(meshName);
    setMaterial(materialName);
}

Entity::Entity(Handle<Mesh> meshHandle, const std::string &materialName) :
    collider(false) {
    setMesh(meshHandle);
    setMaterial(materialName);
}

Entity::Entity() : Entity("defaultMesh", "defaultMaterial") {
}

void Entity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    for (auto &ptr : subEntities)
        ptr->addToRenderQueue(renderQueue, viewport, renderTarget);

#ifdef ENABLE_BV_GIZMOS
    if(collider && renderTarget.getRenderPhase() == RenderPhase::COLOR) {
        RenderOperation &ro = renderQueue.addRenderOperation();
        ro.renderTarget = &renderTarget;
        ro.vertexData = const_cast<VertexData *>(boundingVolumeMesh->getSubMeshes()[0]->getVertexData());
        ro.indexData = const_cast<IndexData *>(boundingVolumeMesh->getSubMeshes()[0]->getIndexData());
        ro.renderable = this;
        ro.viewport = &viewport;
        ro.passId = 0;
        ro.material = boundingVolumeMaterial.get();
    }
#endif
}

void Entity::preRender(Renderer &renderer, const RenderOperation &renderOperation) {
#ifdef ENABLE_BV_GIZMOS
    if(collider) {
        glm::vec3 pos;
        glm::vec3 scale;

        if(worldSpaceBoundingVolume->getType() == BoundingVolumeType::AABB) {
            AABB &aabb = *dynamic_cast<AABB*>(worldSpaceBoundingVolume.get());
            pos = aabb.getCenter();
            scale = aabb.getDimensions();
        }
        else if(worldSpaceBoundingVolume->getType() == BoundingVolumeType::SPHERE) {
            BoundingSphere &bs = *dynamic_cast<BoundingSphere*>(worldSpaceBoundingVolume.get());
            pos = bs.getCenter();
            scale = glm::vec3(bs.getRadius() * 2.0f);
        }

        glm::mat4 transform(scale.x, 0.0f, 0.0f, 0.0f,
                            0.0f, scale.y, 0.0f, 0.0f,
                            0.0f, 0.0f, scale.z, 0.0f,
                            pos.x, pos.y, pos.z, 1.0f);

        renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
                                                       *renderOperation.viewport,
                                                       transform);
    }
#endif
}

void Entity::render(Renderer &renderer, const RenderOperation &renderOperation) {
#ifdef ENABLE_BV_GIZMOS
    if(collider) {
        renderer.renderIndexed(renderOperation.material->getRenderMode(), *renderOperation.vertexData, *renderOperation.indexData);
    }
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

void Entity::updateWorldSpaceBoundingVolume() {
    if(collider && isAttachedToSceneNode()) {
        auto clone = mesh->getBoundingVolume().clone();
        worldSpaceBoundingVolume.swap(clone);
        worldSpaceBoundingVolume->transform(getLocalToWorldTransform());
    }
}

void Entity::setNonCollider() {
#ifdef ENABLE_BV_GIZMOS
    if(collider) {
        boundingVolumeMaterial.invalidate();
        boundingVolumeMesh.invalidate();
        worldSpaceBoundingVolume.reset();
    }
#endif

    collider = false;
}

void Entity::setAsCollider(const std::string &colliderName) {
    this->colliderName = colliderName;
    worldSpaceBoundingVolume = mesh->getBoundingVolume().clone();
    worldSpaceBoundingVolume->empty();

#ifdef ENABLE_BV_GIZMOS
    if(!collider) {
        if(worldSpaceBoundingVolume->getType() == BoundingVolumeType::AABB) {
            boundingVolumeMesh = Root::getInstance().getMeshManager().getAABBGizmo();
            boundingVolumeMaterial = Root::getInstance().getMaterialManager().get("aabbGizmoMaterial");
        }
        else if(worldSpaceBoundingVolume->getType() == BoundingVolumeType::SPHERE) {
            boundingVolumeMesh = Root::getInstance().getMeshManager().get("sphere");
            boundingVolumeMaterial = Root::getInstance().getMaterialManager().get("sphereGizmoMaterial");
        }
    }
#endif

    collider = true;
}

void Entity::onCollision(Entity &other) {
}

void Entity::onSubEntityPreRender(SubEntity &subEntity, Renderer &renderer, const RenderOperation &renderOperation) {
}

void Entity::onSubEntityRender(SubEntity &subEntity, Renderer &renderer, const RenderOperation &renderOperation) {
}


/////////////////////////////////
// Callbacks from SceneManager
/////////////////////////////////
void Entity::onFrameStart(float timePassed) {
    updateWorldSpaceBoundingVolume();
}

void Entity::onFrameRenderingQueued(float timePassed) {
}

void Entity::onFrameEnd(float timePassed) {
}

void Entity::onKeyPress(int key, int modifier) {
}

void Entity::onKeyRelease(int key, int modifier) {
}

void Entity::onKeyRepeat(int key, int modifier) {
}

void Entity::onCursorMove(int x, int y) {
}

void Entity::onButtonPressed(int x, int y, int button, int modifiers) {
}

void Entity::onButtonReleased(int x, int y, int button, int modifiers) {
}
