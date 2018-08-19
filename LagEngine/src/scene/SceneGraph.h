#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "SceneNode.h"

namespace Lag {

    class Scene;

    /*
    * Hierarchical representation of a scene.
    * Responsible for allocation and deallocation of all the SceneNodes.
    *
    * TODO: optimize memory arrangment of the nodes, for traversal efficiency.
    */
    class SceneGraph {
    public:
        explicit SceneGraph(Scene &scene);
        void clear();

        SceneNode &createSceneNode(const std::string &name);

        SceneNode *getSceneNode(const std::string &name) const;
        SceneNode &getRootSceneNode();

        inline uint32 getNodeCount() const { return static_cast<uint32>(nodes.size()); }

    private:
        Scene &scene;

        //All created nodes by name
        std::unordered_map<std::string, std::unique_ptr<SceneNode>> nodes;
    };
}
