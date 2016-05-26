#include "SceneNode.h"

#include "SceneGraph.h"
#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Lag;

SceneNode::SceneNode(SceneGraph &sceneGraph) : 
	sceneGraph(sceneGraph), 
	parent(nullptr),
	inheritOrientation(true),
	inheritScale(true)
{
}

SceneNode::~SceneNode()
{
}

SceneNode& SceneNode::createChildSceneNode(const std::string &name)
{
	SceneNode &newNode = sceneGraph.createSceneNode(name);
	addChildSceneNode(newNode);
	return newNode;
}

void SceneNode::addChildSceneNode(const std::string &name)
{
	SceneNode *node = sceneGraph.getSceneNode(name);
	if (node != nullptr)
		addChildSceneNode(*node);
}

void SceneNode::addChildSceneNode(SceneNode &node)
{
	if (node.hasParent())
		node.parent->removeChildSceneNode(node);
	node.parent = this;
	children.push_front(&node);

	node.updateInheritedData();
}

void SceneNode::removeChildSceneNode(const std::string &name)
{
	SceneNode *node = sceneGraph.getSceneNode(name);
	if (node != nullptr)
		removeChildSceneNode(*node);
}

void SceneNode::removeChildSceneNode(SceneNode &node)
{
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

void SceneNode::yaw(float angle, TransformSpace space)
{
	rotate(angle, glm::vec3(0, 1, 0), space);
}

void SceneNode::pitch(float angle, TransformSpace space)
{
	rotate(angle, glm::vec3(1, 0, 0), space);
}

void SceneNode::roll(float angle, TransformSpace space)
{
	rotate(angle, glm::vec3(0, 0, 1), space);
}

void SceneNode::rotate(float angle, const glm::vec3 &axis, TransformSpace space)
{
	glm::quat q = glm::angleAxis(glm::radians(angle), axis);
	rotate(q, space);
}

void SceneNode::rotate(const glm::quat &quaternion, TransformSpace space)
{
	//avoid drift
	glm::quat normQuat = glm::normalize(quaternion);

	switch (space)
	{
	case PARENT:
		transform.orientation = normQuat * transform.orientation;
		break;
	case WORLD:
		transform.orientation = transform.orientation * 
			glm::inverse(transform.inheritedOrientation) *
			normQuat * transform.inheritedOrientation;
		break;
	case LOCAL:
		transform.orientation = transform.orientation * normQuat;
		break;
	}

	localChangeOccured();
}

void SceneNode::scale(const glm::vec3 &scale)
{
	transform.scale *= scale;
	localChangeOccured();
}

void SceneNode::translate(const glm::vec3 &trans, TransformSpace space)
{
	switch (space)
	{
	case LOCAL:
		transform.position += transform.orientation * trans;
		break;
	case WORLD:
		if (parent != nullptr)
		{
			transform.position += (glm::inverse(parent->transform.inheritedOrientation) * trans)
				/ parent->transform.inheritedScale;
		}
		else
		{
			transform.position += trans;
		}
		break;
	case PARENT:
		//Are this semantics OK? Or only: transform.position += trans;?
		transform.position += transform.inheritedOrientation * trans;
		break;
	}
	localChangeOccured();
}

void SceneNode::setOrientation(const glm::quat &quaternion)
{
	transform.orientation = quaternion;
	localChangeOccured();
}

void SceneNode::setOrientation(float w, float x, float y, float z)
{
	transform.orientation.w = w;
	transform.orientation.x = x;
	transform.orientation.y = y;
	transform.orientation.z = z;
	localChangeOccured();
}

void SceneNode::setScale(const glm::vec3 &scale)
{
	transform.scale = scale;
	localChangeOccured();
}

void SceneNode::setPosition(const glm::vec3 &pos)
{
	transform.position = pos;
	localChangeOccured();
}

void SceneNode::localChangeOccured()
{
	transform.finalTransformOutOfDate = true;
	transform.finalInverseTransformOutOfDate = true;
	notifyChildrenOfDataChange();
}

void SceneNode::notifyChildrenOfDataChange()
{
	for (SceneNode* ch : children)
		ch->updateInheritedData();
}

void SceneNode::updateInheritedData()
{
	transform.inheritedPosition = parent->transform.inheritedPosition + parent->transform.position;
	transform.inheritedOrientation = parent->transform.inheritedOrientation * parent->transform.orientation;
	transform.inheritedScale = parent->transform.inheritedScale * parent->transform.scale;

	transform.finalTransformOutOfDate = true;
	notifyChildrenOfDataChange();
}

const glm::mat4& SceneNode::getFinalTransform()
{
	if (transform.finalTransformOutOfDate)
	{
		transform.finalTransform = glm::mat4();
		transform.finalTransform = glm::translate(transform.finalTransform, transform.position + transform.inheritedPosition);

		glm::mat4 rot;
		if (inheritOrientation)
			rot = glm::mat4(transform.inheritedOrientation * transform.orientation);
		else
			rot = glm::mat4(transform.orientation);

		transform.finalTransform = transform.finalTransform * rot;

		if (inheritScale)
			transform.finalTransform = glm::scale(transform.finalTransform, transform.scale * transform.inheritedScale);
		else
			transform.finalTransform = glm::scale(transform.finalTransform, transform.scale);

		transform.finalTransformOutOfDate = false;
	}
	return transform.finalTransform;
}

const glm::mat4& SceneNode::getFinalInverseTransform()
{
	if (transform.finalInverseTransformOutOfDate)
	{
		transform.finalInverseTransform = glm::mat4();

		if (inheritScale)
			transform.finalInverseTransform = glm::scale(transform.finalInverseTransform, 1.0f / (transform.scale * transform.inheritedScale));
		else
			transform.finalInverseTransform = glm::scale(transform.finalInverseTransform, 1.0f / transform.scale);
		
		glm::mat4 rot;
		if (inheritOrientation)
			rot = glm::mat4(transform.inheritedOrientation * transform.orientation);
		else
			rot = glm::mat4(transform.orientation);

		rot = glm::transpose(rot);
		transform.finalInverseTransform = transform.finalInverseTransform * rot;

		transform.finalInverseTransform = glm::translate(transform.finalInverseTransform, -transform.position - transform.inheritedPosition);
		transform.finalInverseTransformOutOfDate = false;
	}
	return transform.finalInverseTransform;
}

const glm::vec3& SceneNode::getWorldPosition() 
{ 
	transform.tempVec3 = transform.position + transform.inheritedPosition;
	return transform.tempVec3; 
}

const glm::quat& SceneNode::getWorldOrientation()
{
	if (inheritOrientation)
	{
		transform.tempQuat = transform.orientation * transform.inheritedOrientation;
		return transform.tempQuat;
	}
	else
		return transform.orientation;
}

const glm::vec3& SceneNode::getWorldScale()
{
	if (inheritScale)
	{
		transform.tempVec3 = transform.scale * transform.inheritedScale;
		return transform.tempVec3;
	}
	else
		return transform.scale;
}