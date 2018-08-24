#include "SceneNode.h"

#include "glm/glm.hpp"
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

//////////////////////////////////
//Transforms
/////////////////////////////////

void SceneNode::yaw(float angle, TransformSpace rotationSpace) {
    rotate(angle, glm::vec3(0, 1, 0), rotationSpace);
}

void SceneNode::pitch(float angle, TransformSpace rotationSpace) {
    rotate(angle, glm::vec3(1, 0, 0), rotationSpace);
}

void SceneNode::roll(float angle, TransformSpace rotationSpace) {
    rotate(angle, glm::vec3(0, 0, 1), rotationSpace);
}

void SceneNode::rotate(float angle, const glm::vec3 &axis, TransformSpace rotationSpace) {
    glm::quat q = glm::angleAxis(glm::radians(angle), axis);
    rotate(q, rotationSpace);
}

void SceneNode::rotate(const glm::quat &quaternion, TransformSpace rotationSpace) {
    if(isAttached()) {
        const glm::mat3 toParent = pickCorrectTransformToParentSpace(rotationSpace);
        glm::vec3 newAxis = toParent * glm::axis(quaternion);
        transform.orientation = glm::angleAxis(glm::angle(quaternion), newAxis) * transform.orientation;
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

void SceneNode::translate(const glm::vec3 &trans, TransformSpace transSpace) {
    if(isAttached()) {
        const glm::mat3 toParent = pickCorrectTransformToParentSpace(transSpace);
        transform.position += (toParent * trans);
        localChangeOccured();
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneNode", "Trying to scale a non-attached node. Ignoring.");
    }
}

void SceneNode::setOrientation(const glm::quat &quaternion, TransformSpace rotationSpace) {
    const glm::mat3 toParent = pickCorrectTransformToParentSpace(rotationSpace);
    glm::vec3 newAxis = toParent * glm::axis(quaternion);
    transform.orientation = glm::angleAxis(glm::angle(quaternion), newAxis);
    localChangeOccured();
}

void SceneNode::setScale(const glm::vec3 &scale) {
    transform.scale = scale;
    localChangeOccured();
}

void SceneNode::setPosition(const glm::vec3 &pos, TransformSpace positionSpace) {
    const glm::mat3 toParent = pickCorrectTransformToParentSpace(positionSpace);
    transform.position = (toParent * pos);
    localChangeOccured();
}

void SceneNode::lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up, TransformSpace space) {
    const glm::mat4 toParentMatrix = pickCorrectTransformToParentSpace(space);

    glm::vec3 dir = glm::normalize(pos - center);
    glm::vec3 side = glm::normalize(glm::cross(up, dir));
    glm::vec3 newUp = glm::normalize(glm::cross(dir, side));

    glm::mat3 rot(side, newUp, dir);

    transform.orientation = glm::quat(toParentMatrix * glm::mat4(rot));
    transform.position = toParentMatrix * glm::vec4(pos, 1.0f);

    localChangeOccured();
}

void SceneNode::localChangeOccured() {
    transform.localToWorldTransform.setDirty();
    transform.worldToLocalTransform.setDirty();
    transform.localToWorldNormalTransform.setDirty();
    notifyChildrenOfDataChange();
}

void SceneNode::notifyChildrenOfDataChange() {
    for (SceneNode *ch : children)
        ch->updateInheritedData();
}

void SceneNode::updateInheritedData() {
    transform.inheritedPosition = parent->getPositionWorldSpace();
    transform.inheritedOrientation = parent->getOrientationWorldSpace();
    transform.inheritedScale = parent->transform.inheritedScale * parent->getScaleLocalSpace();
    localChangeOccured();
}

const glm::mat4& SceneNode::getLocalToWorldTransform() {
    if (transform.localToWorldTransform.isDirty)
        transform.localToWorldTransform.update(getParentToWorldTransform() * getLocalToParentTransform());
    return *transform.localToWorldTransform;
}

const glm::mat4& SceneNode::getWorldToLocalTransform() {
    if (transform.worldToLocalTransform.isDirty)
        transform.worldToLocalTransform.update(getParentToLocalTransform() * getWorldToParentTransform());
    return *transform.worldToLocalTransform;
}

const glm::mat3& SceneNode::getLocalToWorldNormalTransform() {
    if(transform.localToWorldNormalTransform.isDirty) {
        if(transform.scale.x == transform.scale.y && transform.scale.x == transform.scale.z)
            transform.localToWorldNormalTransform.update(glm::mat3(getLocalToWorldTransform()));
        else
            transform.localToWorldNormalTransform.update(glm::transpose(glm::mat3(getWorldToLocalTransform())));
    }
    return *transform.localToWorldNormalTransform;
}

void SceneNode::setInheritOrientation(bool b) {
    inheritOrientation = b;
    updateInheritedData();
}

void Lag::SceneNode::setInheritScale(bool b) {
    inheritScale = b;
    updateInheritedData();
    notifyChildrenOfDataChange();
}

glm::mat4 SceneNode::getLocalToParentTransform() const {
    glm::mat4 mat(1.0f);
    mat = glm::translate(mat, transform.position);
    mat *= glm::mat4(const_cast<glm::quat&>(transform.orientation));
    mat = glm::scale(mat, transform.scale);
    return mat;
}

glm::mat4 SceneNode::getParentToLocalTransform() const {
    glm::mat4 mat(1.0f);
    mat = glm::scale(mat, 1.0f / transform.scale);
    mat *= glm::transpose(glm::mat4(const_cast<glm::quat&>(transform.orientation)));
    mat = glm::translate(mat, -transform.position);
    return mat;
}

glm::mat4 SceneNode::getParentToWorldTransform() const {
    glm::mat4 mat(1.0f);
    mat = glm::translate(mat, transform.inheritedPosition);
    if(inheritOrientation)
        mat *= glm::mat4(const_cast<glm::quat&>(transform.inheritedOrientation));
    if(inheritScale)
        mat = glm::scale(mat, transform.inheritedScale);
    return mat;
}

glm::mat4 SceneNode::getWorldToParentTransform() const {
    glm::mat4 mat(1.0f);
    if(inheritScale)
        mat = glm::scale(mat, 1.0f / transform.inheritedScale);
    if(inheritOrientation)
        mat *= glm::transpose(glm::mat4(const_cast<glm::quat&>(transform.inheritedOrientation)));
    mat = glm::translate(mat, -transform.inheritedPosition);
    return mat;
}

glm::mat4 SceneNode::pickCorrectTransformToParentSpace(TransformSpace from) {
    switch (from) {
        case TransformSpace::PARENT:
            return glm::mat4(1.0);
        case TransformSpace::WORLD:
            return getWorldToParentTransform();
        case TransformSpace::LOCAL:
            return getLocalToParentTransform();
    }
    return glm::mat4(1.0f);
}

glm::vec3 Lag::SceneNode::getScaleLocalSpace() const {
    if(inheritScale)
        return transform.scale * transform.inheritedScale;
    else
        return transform.scale;
}

glm::vec3 SceneNode::getPositionWorldSpace() const {
    return glm::vec3(getParentToWorldTransform() * glm::vec4(transform.position, 1.0f));
}

glm::quat SceneNode::getOrientationWorldSpace() const {
    if(isAttached() && inheritOrientation)
        return parent->transform.inheritedOrientation * transform.orientation;
    else
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}