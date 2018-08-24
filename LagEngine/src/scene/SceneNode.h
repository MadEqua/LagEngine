#pragma once

#include <string>
#include <forward_list>

#include "Types.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Transform.h"

namespace Lag {

    class SceneObject;
    class SceneGraph;

    enum class TransformSpace : uint8 {
        LOCAL, //Node Space
        PARENT,
        WORLD
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

        inline bool isAttached() const { return parent != nullptr; }
        inline SceneNode *getParentSceneNode() const { return parent; }

        //Transforms
        void yaw(float angle, TransformSpace rotationSpace = TransformSpace::LOCAL);
        void pitch(float angle, TransformSpace rotationSpace = TransformSpace::LOCAL);
        void roll(float angle, TransformSpace rotationSpace = TransformSpace::LOCAL);

        void rotate(float angle, const glm::vec3 &axis, TransformSpace rotationSpace = TransformSpace::LOCAL);
        void rotate(const glm::quat &quaternion, TransformSpace rotationSpace = TransformSpace::LOCAL);

        //Scale always on local space
        void scale(const glm::vec3 &scale);

        void translate(const glm::vec3 &trans, TransformSpace transSpace = TransformSpace::LOCAL);

        void setOrientation(const glm::quat &quaternion, TransformSpace transSpace = TransformSpace::PARENT);
        void setPosition(const glm::vec3 &pos, TransformSpace positionSpace = TransformSpace::PARENT);
        void setScale(const glm::vec3 &scale);

        void lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up, TransformSpace space = TransformSpace::PARENT);

        //Node in relation to parent SceneNode
        inline const glm::vec3 &getPositionParentSpace() const { return transform.position; }
        inline const glm::quat &getOrientationParentSpace() const { return transform.orientation; }

        //Node in relation to the world.
        glm::vec3 getPositionWorldSpace() const;
        glm::quat getOrientationWorldSpace() const;

        glm::vec3 getScaleLocalSpace() const;

        const glm::mat4& getLocalToWorldTransform(); //Full transform, traversing the graph
        const glm::mat4& getWorldToLocalTransform();
        const glm::mat3& getLocalToWorldNormalTransform(); //Appropriate transform for transforming normals

        void setInheritOrientation(bool b);
        void setInheritScale(bool b);

    private:
        friend class SceneGraph;

        explicit SceneNode(SceneGraph &sceneGraph);

        Transform transform;
        SceneGraph &sceneGraph;
        SceneNode *parent; //May be nullptr when not added to the SceneGraph

        bool inheritOrientation;
        bool inheritScale;

        std::forward_list<SceneNode *> children;

        //set dirty flags and notify children calling updateInheritedData()
        void localChangeOccured();
        void notifyChildrenOfDataChange();

        //Update the inherited position, orientation and scale
        void updateInheritedData();

        glm::mat4 getLocalToParentTransform() const;
        glm::mat4 getParentToLocalTransform() const;

        glm::mat4 getParentToWorldTransform() const;
        glm::mat4 getWorldToParentTransform() const;

        glm::mat4 pickCorrectTransformToParentSpace(TransformSpace from);
    };
}