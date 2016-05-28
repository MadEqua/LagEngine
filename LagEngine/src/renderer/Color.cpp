#include "Color.h"
#include <glm/glm.hpp>

using namespace Lag;

Color::Color()
{
	channels[0] = 1.0f;
	channels[1] = 1.0f;
	channels[2] = 1.0f;
	channels[3] = 1.0f;
}

Color::Color(uint8 v)
{
	channels[0] = static_cast<float>(v) / 255.0f;
	channels[1] = static_cast<float>(v) / 255.0f;
	channels[2] = static_cast<float>(v) / 255.0f;
	channels[3] = 1.0f;
}

Color::Color(uint8 r, uint8 g, uint8 b)
{
	channels[0] = static_cast<float>(r) / 255.0f;
	channels[1] = static_cast<float>(g) / 255.0f;
	channels[2] = static_cast<float>(b) / 255.0f;
	channels[3] = 1.0f;
}

Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
{
	channels[0] = static_cast<float>(r) / 255.0f;
	channels[1] = static_cast<float>(g) / 255.0f;
	channels[2] = static_cast<float>(b) / 255.0f;
	channels[3] = static_cast<float>(a) / 255.0f;
}

Color::Color(float v)
{
	channels[0] = v;
	channels[1] = v;
	channels[2] = v;
	channels[3] = 1.0f;
}

Color::Color(float r, float g, float b)
{
	channels[0] = r;
	channels[1] = g;
	channels[2] = b;
	channels[3] = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
	channels[0] = r;
	channels[1] = g;
	channels[2] = b;
	channels[3] = a;
}

Color::~Color()
{
}