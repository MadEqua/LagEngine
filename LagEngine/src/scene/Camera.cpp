#include "Camera.h"

using namespace Lag;

Camera::Camera(uint32 name, float aspectRatio, float nearPlane, float farPlane) :
        SceneObject(name),
        projectionMatrix(1.0f),
        aspectRatio(aspectRatio),
        nearPlane(nearPlane),
        farPlane(farPlane) {
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    computeProjectionMatrix();
}