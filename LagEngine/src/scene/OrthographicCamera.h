#pragma once

#include "Camera.h"

namespace Lag
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(uint32 name, float left, float right, float bottom, float top, float nearPlane, float farPlane);

	protected:
		float left, right;
		float bottom, top;

		virtual void computeProjectionMatrix() override;
	};
}

