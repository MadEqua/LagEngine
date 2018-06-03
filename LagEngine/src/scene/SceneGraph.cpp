#include "SceneGraph.h"

#include "LogManager.h"
#include "SceneNode.h"

using namespace Lag;

SceneGraph::~SceneGraph() {
    clear();
}

void SceneGraph::clear() {
    for (auto &pair : nodes)
        delete pair.second;

    nodes.clear();
}

SceneNode &SceneGraph::createSceneNode(const std::string &name) {
    auto *newNode = new SceneNode(*this);

    auto it = nodes.find(name);
    if (it != nodes.end()) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                      "SceneGraph", "Creating a SceneNode with an already taken name: " + name +
                                                    ". Replacing the already existent one.");
        delete it->second;
    }

    nodes[name] = newNode;
    return *newNode;
}

SceneNode *SceneGraph::getSceneNode(const std::string &name) const {
    auto it = nodes.find(name);
    if (it != nodes.end())
        return it->second;
    else
        return nullptr;
}

SceneNode &SceneGraph::getRootSceneNode() {
    auto it = nodes.find("root");
    if (it == nodes.end()) {
        auto *root = new SceneNode(*this);
        nodes["root"] = root;
        return *root;
    }
    else
        return *it->second;
}