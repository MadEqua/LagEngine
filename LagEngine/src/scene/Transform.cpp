#include "Transform.h"

using namespace Lag;

Transform::Transform() :
        finalTransformOutOfDate(true),
        finalInverseTransformOutOfDate(true),
        normalTransformOutOfDate(true),

        position(0.0f),
        orientation(1.0f, 0.0f, 0.0f, 0.0f),
        scale(1.0f),

        inheritedPosition(0.0),
        inheritedOrientation(1.0f, 0.0f, 0.0f, 0.0f),
        inheritedScale(1.0f) {
}