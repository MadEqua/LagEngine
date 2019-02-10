#pragma once

#include "PerspectiveCamera.h"

#include "IFrameListener.h"
#include "CubicHermiteInterpolator.h"

#include "glm/glm.hpp"

#include <vector>


namespace Lag {
    class Scene;
    class PerspectiveCamera;
    class SceneNode;

    class InterpolatedCamera : public IFrameListener {

    public:
        InterpolatedCamera(Scene &scene, float fovy, float nearPlane, float farPlane,
                           const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &lookAts,
                           float totalSecondsToLoop);
        ~InterpolatedCamera() override;

        inline Camera& getCamera() const { return *camera; }

        void onFrameStart(float timePassed) override;
        void onFrameRenderingQueued(float timePassed) override;
        void onFrameEnd(float timePassed) override;

    protected:
        CubicHermiteInterpolator<glm::vec3> positionsInterpolator;
        CubicHermiteInterpolator<glm::vec3> lookAtsInterpolator;

        const float totalSecondsToLoop;
        float elapsedTime;

        PerspectiveCamera *camera;
        SceneNode *sceneNode;
    };
}