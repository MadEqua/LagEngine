#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Lag;

Camera::Camera(float fovy, float nearPlane, float farPlane) :
	fieldOfViewY(fovy),
	aspectRatio(1.0f),
	nearPlane(nearPlane),
	farPlane(farPlane)
{
	computeProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	computeProjectionMatrix();
}

void Camera::computeProjectionMatrix()
{
	projectionMatrix = glm::perspective(fieldOfViewY, aspectRatio, nearPlane, farPlane);
}