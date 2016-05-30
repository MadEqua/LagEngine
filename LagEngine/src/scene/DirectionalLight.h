#pragma once

#include "Light.h"
#include <glm/vec3.hpp>

namespace Lag
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const glm::vec3 &direction, const Color &color);
		virtual ~DirectionalLight();

		inline const glm::vec3& getDirection() const { return direction; }

	private:
		glm::vec3 direction;
	};
}