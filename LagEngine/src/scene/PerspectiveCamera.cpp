#include "PerspectiveCamera.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace Lag;

PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fieldOfViewY,
                                     float nearPlane, float farPlane) :
        fieldOfViewY(fieldOfViewY),
        Camera(aspectRatio, nearPlane, farPlane) {
    PerspectiveCamera::computeProjectionMatrix();
}

void PerspectiveCamera::computeProjectionMatrix() {
    projectionMatrix = glm::perspective(fieldOfViewY, aspectRatio, nearPlane, farPlane);
}