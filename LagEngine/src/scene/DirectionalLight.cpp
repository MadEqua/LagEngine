#include "DirectionalLight.h"

using namespace Lag;

DirectionalLight::DirectionalLight(const glm::vec3 & direction, const Color & color) :  
	 Light(color), direction(direction)
{
}

DirectionalLight::~DirectionalLight()
{
}