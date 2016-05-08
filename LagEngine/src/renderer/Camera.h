#pragma once

#include "SceneObject.h"
#include "Frustum.h"

namespace Lag
{
	class Camera : public SceneObject
	{
	public:
		Camera();
		~Camera();

	private:
		Frustum frustum;
	};
}

