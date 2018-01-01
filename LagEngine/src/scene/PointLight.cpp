#include "PointLight.h"

using namespace Lag;

PointLight::PointLight(uint32 name, const Color &color, const glm::vec3 &attenuation, bool castShadow) :
	Light(name, color, castShadow), 
	attenuation(attenuation)
{
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	attenuation[0] = constant;
	attenuation[1] = linear;
	attenuation[2] = quadratic;
}