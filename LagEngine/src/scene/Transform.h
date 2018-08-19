#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

namespace Lag {
    /*
    * Container for SceneNode spatial data. Orientation, Position and Scale.
    */
    class Transform {
    private:
        friend class SceneNode;

        Transform();

        //Relative to parent, always up to date.
        glm::vec3 position;
        glm::quat orientation;
        glm::vec3 scale;

        //Values computed from the node hierarchy, always up to date. Accumulate parent node data.
        glm::vec3 inheritedPosition;
        glm::quat inheritedOrientation;
        glm::vec3 inheritedScale;

        //Cache, may be dirty.
        glm::mat4 localToWorldTransform;
        bool localToWorldTransformDirty;

        //Cache, may be dirty.
        glm::mat4 worldToLocalTransform;
        bool worldToLocalTransformDirty;

        //Appropriate matrix to transform normals
        //(equals localToWorldTransform if the scale is uniform)
        glm::mat3 localToWorldNormalTransform;
        bool localToWorldNormalTransformDirty;

        //for returning
        glm::vec3 tempVec3;
        glm::quat tempQuat;
    };
}