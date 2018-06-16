#pragma once

#include "Types.h"

namespace Lag {
    class Color {
    public:
        Color();

        explicit Color(int v);
        Color(int r, int g, int b);
        Color(int r, int g, int b, int a);

        explicit Color(float v);
        Color(float r, float g, float b);
        Color(float r, float g, float b, float a);

        inline const float *getRGBAfloat() const { return channels; }
        inline float r() const { return channels[0]; }
        inline float g() const { return channels[1]; }
        inline float b() const { return channels[2]; }
        inline float a() const { return channels[3]; }

        bool operator==(const Color &rhs) const;
        bool operator!=(const Color &rhs) const;

    private:
        float channels[4];
    };
}