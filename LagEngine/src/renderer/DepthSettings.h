#pragma once

#include "ComparisionFunction.h"

namespace Lag {
    struct DepthSettings {
        DepthSettings();
        DepthSettings(bool enableDepthTest, bool enableDepthWriting, bool enableDepthClamping,
                      ComparisionFunction comparisionFunction);

        bool operator==(const DepthSettings &rhs) const;
        bool operator!=(const DepthSettings &rhs) const;

        bool enableDepthTest; //disabling this will disable all depth buffer functionality
        bool enableDepthWriting; //depth mask
        bool enableDepthClamping;
        ComparisionFunction comparisionFunction; //to determine if the fragment passed the test
    };
}
