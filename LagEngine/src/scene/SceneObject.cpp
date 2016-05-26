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

const glm::mat4& SceneObject::getTransform() const
{
	return parentSceneNode != nullptr ? 
		parentSceneNode->getFinalTransform() : temp;
}

const glm::mat4& SceneObject::getInverseTransform() const
{
	return parentSceneNode != nullptr ?
		parentSceneNode->getFinalInverseTransform() : temp;
}