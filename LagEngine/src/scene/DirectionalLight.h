#pragma once

#include "Light.h"
#include <glm/vec3.hpp>

namespace Lag
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(uint32 name, const glm::vec3 &direction, const Color &color, bool castShadow = true);

		inline const glm::vec3& getDirection() const { return direction; }

	private:
		glm::vec3 direction;
	};
}