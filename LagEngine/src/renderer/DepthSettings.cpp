#include "DepthSettings.h"

using namespace Lag;

DepthSettings::DepthSettings() : enableDepthTest(true),
                                 enableDepthWriting(true),
                                 enableDepthClamping(false),
                                 comparisionFunction(ComparisionFunction::LESS) {}

DepthSettings::DepthSettings(bool enableDepthTest, bool enableDepthWriting, bool enableDepthClamping,
                                         ComparisionFunction comparisionFunction) :
        enableDepthTest(enableDepthTest),
        enableDepthWriting(enableDepthWriting),
        enableDepthClamping(enableDepthClamping),
        comparisionFunction(comparisionFunction) {}

bool DepthSettings::operator==(const DepthSettings &rhs) const {
    return enableDepthTest == rhs.enableDepthTest &&
           enableDepthWriting == rhs.enableDepthWriting &&
           enableDepthClamping == rhs.enableDepthClamping &&
           comparisionFunction == rhs.comparisionFunction;
}

bool DepthSettings::operator!=(const DepthSettings &rhs) const {
    return !(rhs == *this);
}
