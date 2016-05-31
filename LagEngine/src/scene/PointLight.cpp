#include "PointLight.h"

using namespace Lag;

PointLight::PointLight(const Color &color, const glm::vec3 &attenuation) :
	Light(color), 
	attenuation(attenuation)
{
}

PointLight::~PointLight()
{
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	attenuation[0] = constant;
	attenuation[1] = linear;
	attenuation[2] = quadratic;
}