#include "Light.h"

using namespace Lag;

Light::Light(uint32 name, const Color &color, bool castShadow) :
        SceneObject(name),
        color(color),
        castShadow(castShadow) {
}