#include "SceneNode.h"

#include "SceneGraph.h"

using namespace Lag;

SceneNode::SceneNode(SceneGraph &sceneGraph) : sceneGraph(sceneGraph), parent(nullptr)
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

void SceneNode::attachSceneObject(SceneObject &sceneObject)
{
	attachedObjects.push_front(&sceneObject);
}

void SceneNode::detachSceneObject(SceneObject &sceneObject)
{
	attachedObjects.remove(&sceneObject);
}