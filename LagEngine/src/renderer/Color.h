#pragma once

#include "../Types.h"

namespace Lag
{
	class Color
	{
	public:
		Color();

		Color(int v);
		Color(int r, int g, int b);
		Color(int r, int g, int b, int a);

		Color(float v);
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);

		inline const float* getRGBAfloat() const { return channels; }

	private:
		float channels[4];
	};
}