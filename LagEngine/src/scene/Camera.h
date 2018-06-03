#pragma once

#include "SceneObject.h"
#include "Frustum.h"

#include "glm/glm.hpp"

namespace Lag {
    /*
    * TODO: use the Frustum
    */
    class Camera : public SceneObject {
    public:
        Camera(uint32 name, float aspectRatio, float nearPlane, float farPlane);

        void setAspectRatio(float aspectRatio);

        inline const glm::mat4 &getProjectionMatrix() const { return projectionMatrix; }

    protected:
        Frustum frustum;

        float aspectRatio;
        float nearPlane, farPlane;
        glm::mat4 projectionMatrix;

        virtual void computeProjectionMatrix() = 0;
    };
}

