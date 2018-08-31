#include "SceneGraph.h"

#include "LogManager.h"
#include "Scene.h"
#include "Entity.h"

using namespace Lag;

SceneGraph::SceneGraph(Scene &scene) : scene(scene) {
}

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

    auto sn = new SceneNode(*this);

#ifdef ENABLE_DEBUG_MACRO
    auto gizmo = scene.createAxisGizmo();
    gizmo->attachToSceneNode(*sn);
#endif

    nodes[name] = std::unique_ptr<SceneNode>(sn);
    return *sn;
}

SceneNode *SceneGraph::getSceneNode(const std::string &name) const {
    auto it = nodes.find(name);
    if (it != nodes.end())
        return it->second.get();
    else
        return nullptr;
}

SceneNode &SceneGraph::getRootSceneNode() {
    auto root = getSceneNode(ROOT_SCENE_NODE);
    if (!root)
        return createSceneNode(ROOT_SCENE_NODE);
    else
        return *root;
}
