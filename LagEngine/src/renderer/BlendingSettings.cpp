#include "BlendingSettings.h"

using namespace Lag;

BlendingSetting::BlendingSetting(BlendingFunction sourceBlendingFunction, BlendingFunction destinationBlendingFunction, BlendingEquation blendingEquation) :
        sourceBlendingFunction(sourceBlendingFunction),
        destinationBlendingFunction(destinationBlendingFunction),
        blendingEquation(blendingEquation) {}

bool BlendingSetting::operator==(const BlendingSetting &rhs) const {
    return sourceBlendingFunction == rhs.sourceBlendingFunction &&
           destinationBlendingFunction == rhs.destinationBlendingFunction &&
           blendingEquation == rhs.blendingEquation;
}

bool BlendingSetting::needsConstantColor() const {
    return sourceBlendingFunction == BlendingFunction::CONSTANT_COLOR ||
           sourceBlendingFunction == BlendingFunction::CONSTANT_ALPHA ||
           sourceBlendingFunction == BlendingFunction::ONE_MINUS_CONSTANT_COLOR ||
           sourceBlendingFunction == BlendingFunction::ONE_MINUS_CONSTANT_ALPHA ||
           destinationBlendingFunction == BlendingFunction::CONSTANT_COLOR ||
           destinationBlendingFunction == BlendingFunction::CONSTANT_ALPHA ||
           destinationBlendingFunction == BlendingFunction::ONE_MINUS_CONSTANT_COLOR ||
           destinationBlendingFunction == BlendingFunction::ONE_MINUS_CONSTANT_ALPHA;
}

bool BlendingSetting::operator!=(const BlendingSetting &rhs) const {
    return !(rhs == *this);
}

BlendingSettings::BlendingSettings() : enableBlending(false) {}

BlendingSettings::BlendingSettings(const BlendingSetting &settingRGB, const BlendingSetting &settingAlpha) :
        enableBlending(true), settingRGB(settingRGB), settingAlpha(settingAlpha) {}

BlendingSettings::BlendingSettings(const BlendingSetting &settingRGB, const BlendingSetting &settingAlpha, const Color &constantColor) :
        enableBlending(true), settingRGB(settingRGB), settingAlpha(settingAlpha), constantColor(constantColor) {}

bool BlendingSettings::operator==(const BlendingSettings &rhs) const {
    return enableBlending == rhs.enableBlending &&
           settingRGB == rhs.settingRGB &&
           settingAlpha == rhs.settingAlpha &&
           constantColor == rhs.constantColor;
}

bool BlendingSettings::operator!=(const BlendingSettings &rhs) const {
    return !(rhs == *this);
}
