#include "SceneObject.h"

#include "SceneNode.h"

using namespace Lag;


SceneObject::SceneObject() : parentSceneNode(nullptr) {
}

glm::vec3 SceneObject::getWorldPosition() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getPositionWorldSpace() : glm::vec3(1.0f);
}

glm::mat4 SceneObject::getLocalToWorldTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getLocalToWorldTransform() : glm::mat4(1.0f);
}

glm::mat4 SceneObject::getWorldToLocalTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getWorldToLocalTransform() : glm::mat4(1.0f);
}

glm::mat3 SceneObject::getNormalTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getLocalToWorldNormalTransform() : glm::mat3(1.0f);
}