#include "SceneObject.h"

#include "SceneNode.h"

using namespace Lag;

glm::mat4 SceneObject::defaultMat4 = glm::mat4(1.0f);
glm::mat3 SceneObject::defaultMat3 = glm::mat3(1.0f);
glm::vec3 SceneObject::defaultVec = glm::vec3(1.0f);

SceneObject::SceneObject(uint32 name) :
        name(name),
        parentSceneNode(nullptr) {
}

const glm::vec3 &SceneObject::getWorldPosition() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getWorldPosition() : defaultVec;
}

const glm::mat4 &SceneObject::getLocalToWorldTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getLocalToWorldTransform() : defaultMat4;
}

const glm::mat4 &SceneObject::getWorldToLocalTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getWorldToLocalTransform() : defaultMat4;
}

const glm::mat3 &SceneObject::getNormalTransform() const {
    return parentSceneNode != nullptr ?
           parentSceneNode->getLocalToWorldNormalTransform() : defaultMat3;
}