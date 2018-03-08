#include "SceneGraph.h"

#include "../io/log/LogManager.h"
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
	clear();
}

void SceneGraph::clear()
{
	for (auto &pair : nodes)
		delete pair.second;

	nodes.clear();
}

SceneNode& SceneGraph::createSceneNode(const std::string &name)
{
	SceneNode *newNode = new SceneNode(*this);

	auto it = nodes.find(name);
	if (it != nodes.end())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"SceneGraph", "Creating a SceneNode with an already taken name: " + name + ". Replacing the already existent one.");
		delete it->second;
	}

	nodes[name] = newNode;
	return *newNode;
}

SceneNode* SceneGraph::getSceneNode(const std::string &name) const
{
	auto it = nodes.find(name);
	if (it != nodes.end())
		return it->second;
	else
		return nullptr;
}