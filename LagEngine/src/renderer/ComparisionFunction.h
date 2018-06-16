#pragma once

#include "Types.h"

namespace Lag {
    enum class ComparisionFunction : uint8 {
        NEVER,
        ALWAYS,
        LESS,
        LESS_OR_EQUAL,
        GREATER,
        GREATER_OR_EQUAL,
        EQUAL,
        NOT_EQUAL
    };
}