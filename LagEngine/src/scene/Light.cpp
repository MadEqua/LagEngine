#include "Light.h"

using namespace Lag;

Light::Light(const Color &color, bool castShadow) :
        color(color),
        castShadow(castShadow) {
}