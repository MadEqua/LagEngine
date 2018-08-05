#include "SceneGraph.h"

#include "LogManager.h"

using namespace Lag;

void SceneGraph::clear() {
    nodes.clear();
}

SceneNode &SceneGraph::createSceneNode(const std::string &name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneGraph",
                                      "Creating a SceneNode with an already taken name: " + name +
                                      ". Replacing the already existent one.");
    }

    nodes[name] = std::unique_ptr<SceneNode>(new SceneNode(*this));
    return *nodes[name];
}

SceneNode *SceneGraph::getSceneNode(const std::string &name) const {
    auto it = nodes.find(name);
    if (it != nodes.end())
        return it->second.get();
    else
        return nullptr;
}

SceneNode &SceneGraph::getRootSceneNode() {
    auto it = nodes.find(ROOT_SCENE_NODE);
    if (it == nodes.end()) {
        nodes[ROOT_SCENE_NODE] = std::unique_ptr<SceneNode>(new SceneNode(*this));
        return *nodes[ROOT_SCENE_NODE];
    }
    else
        return *it->second;
}