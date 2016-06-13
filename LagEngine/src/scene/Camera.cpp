#include "Camera.h"

using namespace Lag;

Camera::Camera(uint32 name, float aspectRatio, float nearPlane, float farPlane) :
	SceneObject(name),
	aspectRatio(aspectRatio),
	nearPlane(nearPlane),
	farPlane(farPlane)
{
}

Camera::~Camera()
{
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	computeProjectionMatrix();
}