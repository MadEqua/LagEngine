#include "SceneNode.h"

#include "glm/gtc/matrix_transform.hpp"

#include "SceneGraph.h"
#include "SceneObject.h"
#include "LogManager.h"

using namespace Lag;

SceneNode::SceneNode(SceneGraph &sceneGraph) :
        sceneGraph(sceneGraph),
        parent(nullptr),
        inheritOrientation(true),
        inheritScale(true) {
}

SceneNode &SceneNode::createChildSceneNode(const std::string &name) {
    SceneNode &newNode = sceneGraph.createSceneNode(name);
    addChildSceneNode(newNode);
    return newNode;
}

void SceneNode::addChildSceneNode(const std::string &name) {
    SceneNode *node = sceneGraph.getSceneNode(name);
    if (node != nullptr)
        addChildSceneNode(*node);
}

void SceneNode::addChildSceneNode(SceneNode &node) {
    if (node.isAttached())
        node.parent->removeChildSceneNode(node);
    node.parent = this;
    children.push_front(&node);

    node.updateInheritedData();
}

void SceneNode::removeChildSceneNode(const std::string &name) {
    SceneNode *node = sceneGraph.getSceneNode(name);
    if (node != nullptr)
        removeChildSceneNode(*node);
}

void SceneNode::removeChildSceneNode(SceneNode &node) {
    children.remove(&node);
    node.parent = nullptr;
}

/*void SceneNode::attachSceneObject(SceneObject &sceneObject)
{
	sceneObject.attachToSceneNode(*this);
	attachedObjects.push_front(&sceneObject);
}

void SceneNode::detachSceneObject(SceneObject &sceneObject)
{
	sceneObject.detachFromSceneNode();
	attachedObjects.remove(&sceneObject);
}*/

//////////////////////////////////
//Transforms
/////////////////////////////////

void SceneNode::yaw(float angle, TransformSpace relativeTo) {
    rotate(angle, glm::vec3(0, 1, 0), relativeTo);
}

void SceneNode::pitch(float angle, TransformSpace relativeTo) {
    rotate(angle, glm::vec3(1, 0, 0), relativeTo);
}

void SceneNode::roll(float angle, TransformSpace relativeTo) {
    rotate(angle, glm::vec3(0, 0, 1), relativeTo);
}

void SceneNode::rotate(float angle, const glm::vec3 &axis, TransformSpace relativeTo) {
    glm::quat q = glm::angleAxis(glm::radians(angle), axis);
    rotate(q, relativeTo);
}

void SceneNode::rotate(const glm::quat &quaternion, TransformSpace relativeTo) {
    if(isAttached()) {
        //avoid drift
        glm::quat normQuat = glm::normalize(quaternion);

        switch (relativeTo) {
            case TransformSpace::LOCAL:
                transform.orientation = transform.orientation * normQuat;
                break;
            case TransformSpace::WORLD:
                transform.orientation = glm::inverse(transform.inheritedOrientation) *
                                        normQuat * transform.inheritedOrientation *
                                        transform.orientation;
                break;
            case TransformSpace::PARENT:
                transform.orientation = normQuat * transform.orientation;
                break;
        }

        localChangeOccured();
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneNode", "Trying to rotate a non-attached node. Ignoring.");
    }
}

void SceneNode::scale(const glm::vec3 &scale) {
    if (isAttached()) {
        transform.scale *= scale;
        localChangeOccured();
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneNode", "Trying to scale a non-attached node. Ignoring.");
    }
}

void SceneNode::translate(const glm::vec3 &trans, TransformSpace relativeTo) {
    if(isAttached()) {
        switch (relativeTo) {
            case TransformSpace::LOCAL:
                transform.position += transform.orientation * trans;
                break;
            case TransformSpace::WORLD:
                transform.position += glm::inverse(parent->transform.inheritedOrientation) * trans;
                break;
            case TransformSpace::PARENT:
                transform.position += trans;
                break;
        }
        localChangeOccured();
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneNode", "Trying to scale a non-attached node. Ignoring.");
    }
}

void SceneNode::setOrientation(const glm::quat &quaternion) {
    transform.orientation = quaternion;
    localChangeOccured();
}

void SceneNode::setOrientation(float w, float x, float y, float z) {
    transform.orientation.w = w;
    transform.orientation.x = x;
    transform.orientation.y = y;
    transform.orientation.z = z;
    localChangeOccured();
}

void SceneNode::setScale(const glm::vec3 &scale) {
    transform.scale = scale;
    localChangeOccured();
}

void SceneNode::setPosition(const glm::vec3 &pos) {
    transform.position = pos;
    localChangeOccured();
}

void SceneNode::lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up) {
    transform.position = pos;

    glm::vec3 dir = glm::normalize(pos - center);
    glm::vec3 side = glm::normalize(glm::cross(up, dir));
    glm::vec3 newUp = glm::normalize(glm::cross(dir, side));

    glm::mat3 rot(side, newUp, dir);

    transform.orientation = glm::quat(rot);
    localChangeOccured();
}

void SceneNode::localChangeOccured() {
    transform.localToWorldTransformDirty = true;
    transform.worldToLocalTransformDirty = true;
    transform.localToWorldNormalTransformDirty = true;
    notifyChildrenOfDataChange();
}

void SceneNode::notifyChildrenOfDataChange() {
    for (SceneNode *ch : children)
        ch->updateInheritedData();
}

void SceneNode::updateInheritedData() {
    transform.inheritedPosition = parent->transform.inheritedPosition + parent->transform.position;
    transform.inheritedOrientation = parent->transform.inheritedOrientation * parent->transform.orientation;
    transform.inheritedScale = parent->transform.inheritedScale * parent->transform.scale;

    localChangeOccured();
}

const glm::mat4 &SceneNode::getLocalToWorldTransform() {
    if (transform.localToWorldTransformDirty) {
        transform.localToWorldTransform = glm::mat4(1.0f);
        transform.localToWorldTransform = glm::translate(transform.localToWorldTransform, transform.position + transform.inheritedPosition);

        glm::mat4 rot(1.0f);
        if (inheritOrientation)
            rot = glm::mat4(transform.inheritedOrientation * transform.orientation);
        else
            rot = glm::mat4(transform.orientation);

        transform.localToWorldTransform = transform.localToWorldTransform * rot;

        if (inheritScale)
            transform.localToWorldTransform = glm::scale(transform.localToWorldTransform, transform.scale * transform.inheritedScale);
        else
            transform.localToWorldTransform = glm::scale(transform.localToWorldTransform, transform.scale);

        transform.localToWorldTransformDirty = false;
    }
    return transform.localToWorldTransform;
}

const glm::mat4 &SceneNode::getWorldToLocalTransform() {
    if (transform.worldToLocalTransformDirty) {
        transform.worldToLocalTransform = glm::mat4(1.0f);

        if (inheritScale)
            transform.worldToLocalTransform = glm::scale(transform.worldToLocalTransform,
                                                         1.0f / (transform.scale * transform.inheritedScale));
        else
            transform.worldToLocalTransform = glm::scale(transform.worldToLocalTransform, 1.0f / transform.scale);

        glm::mat4 rot;
        if (inheritOrientation)
            rot = glm::mat4(transform.inheritedOrientation * transform.orientation);
        else
            rot = glm::mat4(transform.orientation);

        rot = glm::transpose(rot);
        transform.worldToLocalTransform = transform.worldToLocalTransform * rot;

        transform.worldToLocalTransform = glm::translate(transform.worldToLocalTransform,
                                                         -transform.position - transform.inheritedPosition);
        transform.worldToLocalTransformDirty = false;
    }
    return transform.worldToLocalTransform;
}

const glm::mat3 &SceneNode::getLocalToWorldNormalTransform() {
    if (transform.localToWorldNormalTransformDirty) {
        if (transform.scale.x == transform.scale.y && transform.scale.x == transform.scale.z)
            transform.localToWorldNormalTransform = glm::mat3(getLocalToWorldTransform());
        else
            transform.localToWorldNormalTransform = glm::transpose(glm::mat3(getWorldToLocalTransform()));

        transform.localToWorldNormalTransformDirty = false;
    }
    return transform.localToWorldNormalTransform;
}

const glm::vec3 &SceneNode::getWorldPosition() {
    transform.tempVec3 = transform.position + transform.inheritedPosition;
    return transform.tempVec3;
}

const glm::quat &SceneNode::getWorldOrientation() {
    if (inheritOrientation) {
        transform.tempQuat = transform.orientation * transform.inheritedOrientation;
        return transform.tempQuat;
    }
    else
        return transform.orientation;
}

const glm::vec3 &SceneNode::getWorldScale() {
    if (inheritScale) {
        transform.tempVec3 = transform.scale * transform.inheritedScale;
        return transform.tempVec3;
    }
    else
        return transform.scale;
}