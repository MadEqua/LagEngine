#pragma once

#include "Types.h"

#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace Lag {
    class SceneNode;

    /*
    * A SceneObject is anything that can be attached to a SceneNode.
    */
    class SceneObject {
    public:
        SceneObject();
        virtual ~SceneObject() = default;

        inline SceneNode *getParentSceneNode() const { return parentSceneNode; }
        inline bool isAttachedToSceneNode() const { return parentSceneNode != nullptr; }

        inline void attachToSceneNode(SceneNode &node) { parentSceneNode = &node; }
        inline void detachFromSceneNode() { parentSceneNode = nullptr; }

        inline SceneNode *getParentSceneNode() { return parentSceneNode; }

        //Convenience methods, transforms belong to SceneNodes
        const glm::vec3 &getWorldPosition() const;

        const glm::mat4 &getLocalToWorldTransform() const;
        const glm::mat4 &getWorldToLocalTransform() const;
        const glm::mat3 &getNormalTransform() const;

    protected:
        SceneNode *parentSceneNode;

        static glm::mat4 defaultMat4;
        static glm::mat3 defaultMat3;
        static glm::vec3 defaultVec;
    };
}
