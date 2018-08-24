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

        template<class T>
        struct MatrixContainer {
            MatrixContainer() : isDirty(true) {}

            inline void setDirty() { isDirty = true; }
            inline void update(T &newMatrix) { matrix = newMatrix; isDirty = false; }
            bool isDirty;

            inline T& operator*() { return matrix; }
        private:
            T matrix;
        };

        Transform();

        //In parent coordinates/space, always up to date.
        glm::vec3 position;
        glm::quat orientation;

        //In world coordinates/space, always up to date.
        //Accumulated values from Root to Parent.
        glm::vec3 inheritedPosition;
        glm::quat inheritedOrientation;

        //In local space, always up to date.
        //Scale will always be interpreted in local space, even the value inherited from the parent.
        glm::vec3 scale;
        glm::vec3 inheritedScale;

        MatrixContainer<glm::mat4> localToWorldTransform;
        MatrixContainer<glm::mat4> worldToLocalTransform;

        //Appropriate matrix to transform normals
        //(equals localToWorldTransform if the scale is uniform)
        MatrixContainer<glm::mat3> localToWorldNormalTransform;
    };
}