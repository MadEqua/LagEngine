#pragma once

#include "../Types.h"

namespace Lag
{
	class Color
	{
	public:
		Color();

		Color(uint8 v);
		Color(uint8 r, uint8 g, uint8 b);
		Color(uint8 r, uint8 g, uint8 b, uint8 a);

		Color(float v);
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);

		~Color();

		inline const float* getRGBAfloat() const { return channels; }

	private:
		float channels[4];
	};
}