#include "Color.h"

#include <glm/glm.hpp>

using namespace Lag;

Color::Color() {
    channels[0] = 1.0f;
    channels[1] = 1.0f;
    channels[2] = 1.0f;
    channels[3] = 1.0f;
}

Color::Color(uint32 v) {
    glm::vec4 vec = glm::unpackUnorm4x8(v);
    channels[0] = vec[0];
    channels[1] = vec[1];
    channels[2] = vec[2];
    channels[3] = vec[3];
}

Color::Color(uint32 r, uint32 g, uint32 b) {
    channels[0] = static_cast<float>(r) / 255.0f;
    channels[1] = static_cast<float>(g) / 255.0f;
    channels[2] = static_cast<float>(b) / 255.0f;
    channels[3] = 1.0f;
}

Color::Color(uint32 r, uint32 g, uint32 b, uint32 a) {
    channels[0] = static_cast<float>(r) / 255.0f;
    channels[1] = static_cast<float>(g) / 255.0f;
    channels[2] = static_cast<float>(b) / 255.0f;
    channels[3] = static_cast<float>(a) / 255.0f;
}

Color::Color(float v) {
    channels[0] = v;
    channels[1] = v;
    channels[2] = v;
    channels[3] = 1.0f;
}

Color::Color(float r, float g, float b) {
    channels[0] = r;
    channels[1] = g;
    channels[2] = b;
    channels[3] = 1.0f;
}

Color::Color(float r, float g, float b, float a) {
    channels[0] = r;
    channels[1] = g;
    channels[2] = b;
    channels[3] = a;
}

bool Color::operator==(const Color &rhs) const {
    return channels[0] == rhs.channels[0] &&
            channels[1] == rhs.channels[1] &&
            channels[2] == rhs.channels[2] &&
            channels[3] == rhs.channels[3];
}

bool Color::operator!=(const Color &rhs) const {
    return !(rhs == *this);
}

Color& Color::operator*(float v) {
    channels[0] *= v;
    channels[1] *= v;
    channels[2] *= v;
    return *this;
}

uint32 Color::toIntABGR() const {
    glm::vec4 v(channels[0], channels[1], channels[2], channels[3]);
    return glm::packUnorm4x8(v);
}
