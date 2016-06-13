#pragma once

#include "Camera.h"

namespace Lag
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(uint32 name, float aspectRatio, float fieldOfViewY, float nearPlane, float farPlane);
		virtual ~PerspectiveCamera();

	protected:
		float fieldOfViewY;
		virtual void computeProjectionMatrix() override;
	};
}

