#pragma once

#include "Types.h"
#include "Color.h"

namespace Lag {

    enum class BlendingFunction : uint8 {
        ZERO,
        ONE,
        SOURCE_COLOR,
        ONE_MINUS_SOURCE_COLOR,
        DESTINATION_COLOR,
        ONE_MINUS_DESTINATION_COLOR,
        SOURCE_ALPHA,
        ONE_MINUS_SOURCE_ALPHA,
        DESTINATION_ALPHA,
        ONE_MINUS_DESTINATION_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA,
        ALPHA_SATURATE, //(R, G, B) = (f, f, f) with f = min(As, 1 - Ad). A = 1

        //Dual Source Blending (Fragment shader outputting 2 colors to the same buffer)
        SOURCE_1_COLOR,
        ONE_MINUS_SOURCE_1_COLOR,
        SOURCE_1_ALPHA,
        ONE_MINUS_SOURCE_1_ALPHA
    };

    enum class BlendingEquation : uint8 {
        ADD,
        SOURCE_MINUS_DESTINATION,
        DESTINATION_MINUS_SOURCE,

        //Ignores BlendingFunction
        MIN,
        MAX
    };

    //Final Color = BlendingEquation(SourceBlendingFunction * SourceColor, DestinationBlendingFunction * DestinationColor)
    struct BlendingSetting {
        BlendingSetting() {}
        BlendingSetting(BlendingFunction sourceBlendingFunction, BlendingFunction destinationBlendingFunction, BlendingEquation blendingEquation);

        bool operator==(const BlendingSetting &rhs) const;
        bool operator!=(const BlendingSetting &rhs) const;

        bool needsConstantColor() const;

        BlendingFunction sourceBlendingFunction;
        BlendingFunction destinationBlendingFunction;
        BlendingEquation blendingEquation;
    };

    struct BlendingSettings {
        BlendingSettings();
        BlendingSettings(const BlendingSetting &settingRGB, const BlendingSetting &settingAlpha);
        BlendingSettings(const BlendingSetting &settingRGB, const BlendingSetting &settingAlpha, const Color &constantColor);

        bool operator==(const BlendingSettings &rhs) const;
        bool operator!=(const BlendingSettings &rhs) const;

        inline bool needsConstantColor() const { return settingRGB.needsConstantColor() || settingAlpha.needsConstantColor(); }

        bool enableBlending;
        BlendingSetting settingRGB;
        BlendingSetting settingAlpha;
        Color constantColor;
    };
}
