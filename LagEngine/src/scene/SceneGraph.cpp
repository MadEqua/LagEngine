#include "SceneGraph.h"

#include "SceneNode.h"

using namespace Lag;

SceneGraph::SceneGraph()
{
	SceneNode *root = new SceneNode(*this);
	nodes["root"] = root;
	this->root = root;
}

SceneGraph::~SceneGraph()
{
	for (auto &node : nodes)
		delete node.second;
}

SceneNode& SceneGraph::createSceneNode(const std::string &name)
{
	SceneNode *newNode = new SceneNode(*this);
	nodes[name] = newNode;
	return *newNode;
}

SceneNode* SceneGraph::getSceneNode(const std::string &name)
{
	auto it = nodes.find(name);
	if (it != nodes.end())
		return it->second;
	else
		return nullptr;
}