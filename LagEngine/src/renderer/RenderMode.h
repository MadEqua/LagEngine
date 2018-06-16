#pragma once

#include "Types.h"

namespace Lag {
    enum class RenderMode : uint8 {
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        LINES,
        LINE_STRIP,
        LINE_LOOP,
        POINTS,
        PATCHES
    };
}