#pragma once

#include "SceneObject.h"
#include "Frustum.h"
#include <glm/glm.hpp>

namespace Lag
{
	/*
	* TODO: use the Frustum
	*/
	class Camera : public SceneObject
	{
	public:
		Camera(float fovy, float nearPlane, float farPlane);
		virtual ~Camera();

		void setAspectRatio(float aspectRatio);

		inline const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }

	private:
		Frustum frustum;

		float fieldOfViewY;
		float aspectRatio;
		float nearPlane, farPlane;
		glm::mat4 projectionMatrix;

		void computeProjectionMatrix();
	};
}

