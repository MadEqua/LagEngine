#include "SceneObject.h"

#include "SceneNode.h"

using namespace Lag;

SceneObject::SceneObject() :
	parentSceneNode(nullptr)
{
}

SceneObject::~SceneObject()
{
}

const glm::vec3& SceneObject::getWorldPosition() const
{
	return parentSceneNode != nullptr ?
		parentSceneNode->getWorldPosition() : tempVec;
}

const glm::mat4& SceneObject::getTransform() const
{
	return parentSceneNode != nullptr ? 
		parentSceneNode->getFinalTransform() : tempMat4;
}

const glm::mat4& SceneObject::getInverseTransform() const
{
	return parentSceneNode != nullptr ?
		parentSceneNode->getFinalInverseTransform() : tempMat4;
}

const glm::mat3& SceneObject::getNormalTransform() const
{
	return parentSceneNode != nullptr ?
		parentSceneNode->getNormalTransform() : tempMat3;
}