#pragma once

#include "Types.h"
#include <glm/glm.hpp>

namespace Lag {
    class Color {
    public:
        Color();

        //Expecting Packed AGBR
        explicit Color(uint32 v);

        Color(uint32 r, uint32 g, uint32 b);
        Color(uint32 r, uint32 g, uint32 b, uint32 a);

        explicit Color(float v);
        Color(float r, float g, float b);
        Color(float r, float g, float b, float a);

        Color(const glm::vec3 &vec);
        Color(const glm::vec4 &vec);

        inline float r() const { return channels[0]; }
        inline float g() const { return channels[1]; }
        inline float b() const { return channels[2]; }
        inline float alpha() const { return channels[3]; }

        //Good to unpack on GLSL with vec4 c = unpackUnorm4x8(). 
        //Result can be accessed with v.rgba.
        uint32 toIntABGR() const;

        inline glm::vec3 toVec3() const { return glm::vec3(channels[0], channels[1], channels[2]); }
        inline glm::vec4 toVec4() const { return glm::vec4(channels[0], channels[1], channels[2], channels[3]); }

        inline const float *getRGBAfloat() const { return channels; }

        inline void setR(float r) { channels[0] = r; }
        inline void setG(float g) { channels[1] = g; }
        inline void setB(float b) { channels[2] = b; }
        inline void setRGB(float r, float g, float b) { channels[0] = r; channels[1] = g; channels[2] = b; }
        inline void setAlpha(float a) { channels[3] = a; }

        bool operator==(const Color &rhs) const;
        bool operator!=(const Color &rhs) const;
        Color& operator*(float v);

    private:
        //RGBA
        float channels[4];
    };
}