#include "PerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Lag;

PerspectiveCamera::PerspectiveCamera(uint32 name, float aspectRatio, float fieldOfViewY,
                                     float nearPlane, float farPlane) :
        fieldOfViewY(fieldOfViewY),
        Camera(name, aspectRatio, nearPlane, farPlane) {
    PerspectiveCamera::computeProjectionMatrix();
}

void PerspectiveCamera::computeProjectionMatrix() {
    projectionMatrix = glm::perspective(fieldOfViewY, aspectRatio, nearPlane, farPlane);
}