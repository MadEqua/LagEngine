#include "Transform.h"

using namespace Lag;

Transform::Transform() :
	finalTransformOutOfDate(true),
	scale(1.0f),
	inheritedScale(1.0f)
{
}

Transform::~Transform()
{
}