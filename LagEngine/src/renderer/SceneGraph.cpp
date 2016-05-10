#include "SceneGraph.h"

#include "SceneNode.h"
#include "../io/log/LogManager.h"

using namespace Lag;

SceneGraph::SceneGraph()
{
	SceneNode *root = new SceneNode(*this);
	nodeMap["root"] = root;
	this->root = root;

	LogManager::getInstance().log(FILE, NORMAL, INFO, "SceneGraph", "Initialized successfully.");
}

SceneGraph::~SceneGraph()
{
	for (auto &node : nodeMap)
		delete node.second;

	LogManager::getInstance().log(FILE, NORMAL, INFO, "SceneGraph", "Destroyed successfully.");
}

SceneNode& SceneGraph::createSceneNode(const std::string &name)
{
	SceneNode *newNode = new SceneNode(*this);
	nodeMap[name] = newNode;
	return *newNode;
}

SceneNode* SceneGraph::getSceneNode(const std::string &name)
{
	auto it = nodeMap.find(name);
	if (it != nodeMap.end())
		return it->second;
	else
		return nullptr;
}

/*Entity& SceneGraph::createEntity(const std::string &meshFileName)
{
}

Camera& SceneGraph::createCamera()
{

}*/