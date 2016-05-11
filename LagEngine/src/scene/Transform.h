#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/mat4x4.hpp"

namespace Lag
{
	/*
	* Container for SceneNode spatial data. Orientation, Postition and Scale.
	*/
	class Transform
	{
	public:
		Transform();
		~Transform();

		//Relative to parent, always up to date.
		glm::vec3 position;
		glm::quat orientation;
		glm::vec3 scale;

		//Values computed from the node hierarchy, always up to date. Accumulate parent data and its parents.
		glm::vec3 inheritedPosition;
		glm::quat inheritedOrientation;
		glm::vec3 inheritedScale;

		//Final transform in World coordinates. Cache, may be dirty.
		glm::mat4 finalTransform;
		bool finalTransformOutOfDate;

		//for returning
		glm::vec3 tempVec3;
		glm::quat tempQuat;
	};
}