#pragma once

#include <string>
#include <forward_list>

#include "Types.h"

#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Transform.h"

namespace Lag {

    class SceneObject;
    class SceneGraph;

    enum class TransformSpace : uint8 {
        LOCAL, PARENT, WORLD
    };

    /*
    * Nodes that form the SceneGraph hieararchy.
    * Contains a Transform that will be applied to all the attached SceneObjects.
    * SceneNodes can only be created through the SceneGraph.
    * Position is always relative to parent node. Orientation and scale are optionally inherited.
    */
    class SceneNode {
    public:
        SceneNode &createChildSceneNode(const std::string &name);
        void addChildSceneNode(const std::string &name);
        void addChildSceneNode(SceneNode &node);

        void removeChildSceneNode(const std::string &name);
        void removeChildSceneNode(SceneNode &node);

        inline bool hasParent() const { return parent != nullptr; }

        inline SceneNode *getParentSceneNode() const { return parent; }

        //Transforms
        void yaw(float angle, TransformSpace space);
        void pitch(float angle, TransformSpace space);
        void roll(float angle, TransformSpace space);

        void rotate(float angle, const glm::vec3 &axis, TransformSpace space);
        void rotate(const glm::quat &quaternion, TransformSpace space);

        void scale(const glm::vec3 &scale);

        void translate(const glm::vec3 &trans, TransformSpace space);

        void setOrientation(const glm::quat &quaternion);
        void setOrientation(float w, float x, float y, float z);

        void setScale(const glm::vec3 &scale);

        void setPosition(const glm::vec3 &pos);

        void lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up);

        //In relation to parent SceneNode
        inline const glm::vec3 &getLocalPosition() const { return transform.position; }
        inline const glm::quat &getLocalOrientation() const { return transform.orientation; }
        inline const glm::vec3 &getLocalScale() const { return transform.scale; }

        const glm::vec3 &getWorldPosition();
        const glm::quat &getWorldOrientation();
        const glm::vec3 &getWorldScale();

        const glm::mat4 &getFinalTransform(); //Full transform, traversing the graph world coordinates
        const glm::mat4 &getFinalInverseTransform();
        const glm::mat3 &getNormalTransform(); //Appropriate transform for transforming normals

        inline void setInheritOrientation(bool b) { inheritOrientation = b; }
        inline void setInheritScale(bool b) { inheritScale = b; }

    private:
        friend class SceneGraph;

        explicit SceneNode(SceneGraph &sceneGraph);

        Transform transform;
        SceneGraph &sceneGraph;
        SceneNode *parent; //May be nullptr when not added to the SceneGraph

        bool inheritOrientation;
        bool inheritScale;

        std::forward_list<SceneNode *> children;
        //std::forward_list<SceneObject*> attachedObjects;

        //set dirty flags and notify children calling updateInheritedData()
        void localChangeOccured();
        void notifyChildrenOfDataChange();

        //Update the inherited position, orientation and scale
        void updateInheritedData();
    };
}