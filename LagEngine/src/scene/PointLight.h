#pragma once

#include "Light.h"
#include <glm/vec3.hpp>

namespace Lag
{
	class PointLight : public Light
	{
	public:
		PointLight(uint32 name, const Color &color, const glm::vec3 &attenuation, bool castShadow = true);

		void setAttenuation(float constant, float linear, float quadratic);
		inline const float* getAttenuation() const { return reinterpret_cast<const float*>(&attenuation); }

	private:
		glm::vec3 attenuation; //constant, linear, quadratic
	};
}