#pragma once

#include "Light.h"

namespace Lag
{
	class PointLight : public Light
	{
	public:
		PointLight(const Color &color);
		virtual ~PointLight();
	};
}