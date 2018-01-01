#include "Transform.h"

using namespace Lag;

Transform::Transform() :
	finalTransformOutOfDate(true),
	finalInverseTransformOutOfDate(true),
	normalTransformOutOfDate(true),
	scale(1.0f),
	inheritedScale(1.0f)
{
}