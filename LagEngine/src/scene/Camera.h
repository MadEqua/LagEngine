#pragma once

#include "SceneObject.h"
#include "Frustum.h"

namespace Lag
{
	class Camera : public SceneObject
	{
	public:
		Camera();
		virtual ~Camera();

	private:
		Frustum frustum;
	};
}

