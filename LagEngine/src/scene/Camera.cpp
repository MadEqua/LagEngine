#include "Camera.h"

using namespace Lag;

Camera::Camera(float aspectRatio, float nearPlane, float farPlane) :
        projectionMatrix(1.0f),
        aspectRatio(aspectRatio),
        nearPlane(nearPlane),
        farPlane(farPlane) {
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    computeProjectionMatrix();
}