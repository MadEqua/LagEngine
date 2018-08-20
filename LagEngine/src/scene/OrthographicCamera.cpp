#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace Lag;

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top,
                                       float nearPlane, float farPlane) :
        left(left), right(right), bottom(bottom), top(top),
        Camera((abs(right - left)) / (abs(top - bottom)), nearPlane, farPlane) {
    OrthographicCamera::computeProjectionMatrix();
}

void OrthographicCamera::computeProjectionMatrix() {
    float w = abs(right - left);
    float h = w / aspectRatio;

    top = h / 2.0f;
    bottom = -h / 2.0f;

    projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}