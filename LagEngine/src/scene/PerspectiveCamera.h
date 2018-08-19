#pragma once

#include "Camera.h"

namespace Lag {
    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(float aspectRatio, float fieldOfViewY, float nearPlane, float farPlane);

    protected:
        float fieldOfViewY;
        virtual void computeProjectionMatrix() override;
    };
}