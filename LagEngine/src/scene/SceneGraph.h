#pragma once

#include "SceneNode.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace Lag {

    /*
    * Hierarchical representation of a scene.
    * Responsible for allocation and deallocation of all the SceneNodes.
    *
    * TODO: optimize memory arrangment of the nodes, for traversal efficiency.
    */
    class SceneGraph {
    public:
        void clear();

        SceneNode &createSceneNode(const std::string &name);

        SceneNode *getSceneNode(const std::string &name) const;
        SceneNode &getRootSceneNode();

    private:
        //All created nodes by name
        std::unordered_map<std::string, std::unique_ptr<SceneNode>> nodes;
    };
}