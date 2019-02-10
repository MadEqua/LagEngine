#include "InterpolatedCamera.h"

#include "Renderer.h"
#include "Scene.h"

using namespace Lag;

InterpolatedCamera::InterpolatedCamera(Scene &scene, float fovy, float nearPlane, float farPlane,
                                       const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &lookAts,
                                       float totalSecondsToLoop) :
    positionsInterpolator(positions),
    lookAtsInterpolator(lookAts),
    elapsedTime(0.0f),
    totalSecondsToLoop(totalSecondsToLoop) {

    Root &root = Root::getInstance();

    SceneNode &rootNode = scene.getSceneGraph().getRootSceneNode();
    sceneNode = &rootNode.createChildSceneNode("interpolatedCameraSceneNode");

    camera = &scene.createPerspectiveCamera(1.0f, fovy, nearPlane, farPlane);
    camera->attachToSceneNode(*sceneNode);

    root.getRenderer().registerObserver(*this);
}

InterpolatedCamera::~InterpolatedCamera() {
    Root &root = Root::getInstance();
    root.getRenderer().unregisterObserver(*this);
}

void InterpolatedCamera::onFrameStart(float timePassed) {
    elapsedTime += timePassed;

    glm::vec3 position = positionsInterpolator.interpolate(elapsedTime, totalSecondsToLoop);
    glm::vec3 lookAt = lookAtsInterpolator.interpolate(elapsedTime, totalSecondsToLoop);

    sceneNode->lookAt(position, lookAt, glm::vec3(0, 1, 0), TransformSpace::WORLD);
}

void InterpolatedCamera::onFrameRenderingQueued(float timePassed) {
}

void InterpolatedCamera::onFrameEnd(float timePassed) {
}