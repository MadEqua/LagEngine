#include "FreeCamera.h"

#include "Renderer.h"
#include "Keys.h"
#include "Scene.h"
#include "SceneNode.h"
#include "InputManager.h"

using namespace Lag;

FreeCamera::FreeCamera(Scene &scene, float fovy, float nearPlane, float farPlane, float moveSpeed) :
        moveSpeed(moveSpeed) {

    keyVector[0] = keyVector[1] = keyVector[2] = keyVector[3] = false;
    lastCursor[0] = lastCursor[1] = -1;

    Root &root = Root::getInstance();

    SceneNode &rootNode = scene.getSceneGraph().getRootSceneNode();
    cameraYawNode = &rootNode.createChildSceneNode("cameraYawNode");
    cameraPitchNode = &cameraYawNode->createChildSceneNode("cameraPitchNode");
    cameraTranslationNode = &cameraPitchNode->createChildSceneNode("cameraTranslationNode");

    camera = &scene.createPerspectiveCamera(1.0f, fovy, nearPlane, farPlane);
    camera->attachToSceneNode(*cameraTranslationNode);

    root.getRenderer().registerObserver(*this);
    root.getInputManager().registerObserver(static_cast<Lag::IKeyboardListener &>(*this));
    root.getInputManager().registerObserver(static_cast<Lag::ICursorListener &>(*this));
}

FreeCamera::~FreeCamera() {
    Root &root = Root::getInstance();
    root.getRenderer().unregisterObserver(*this);
    root.getInputManager().unregisterObserver(static_cast<Lag::IKeyboardListener &>(*this));
    root.getInputManager().unregisterObserver(static_cast<Lag::ICursorListener &>(*this));
}

void FreeCamera::onKeyPress(int key, int modifier) {
    switch (key) {
        case KEY_W:
            keyVector[0] = true;
            break;
        case KEY_A:
            keyVector[1] = true;
            break;
        case KEY_S:
            keyVector[2] = true;
            break;
        case KEY_D:
            keyVector[3] = true;
            break;
    }
}

void FreeCamera::onKeyRelease(int key, int modifier) {
    switch (key) {
        case KEY_W:
            keyVector[0] = false;
            break;
        case KEY_A:
            keyVector[1] = false;
            break;
        case KEY_S:
            keyVector[2] = false;
            break;
        case KEY_D:
            keyVector[3] = false;
            break;
    }
}

void FreeCamera::onKeyRepeat(int key, int modifier) {
}

void FreeCamera::onCursorMove(int x, int y) {
    int dx, dy;

    if (lastCursor[0] != -1) {
        dx = lastCursor[0] - x;
        cameraYawNode->yaw(static_cast<float>(dx) * 0.1f, TransformSpace::LOCAL);
    }

    if (lastCursor[1] != -1) {
        dy = lastCursor[1] - y;
        cameraPitchNode->pitch(static_cast<float>(dy) * 0.1f, TransformSpace::LOCAL);
    }

    lastCursor[0] = x;
    lastCursor[1] = y;
}

void FreeCamera::onButtonPressed(int x, int y, int button, int modifiers) {
}

void FreeCamera::onButtonReleased(int x, int y, int button, int modifiers) {
}

void FreeCamera::onFrameStart(float timePassed) {
}

void FreeCamera::onFrameRenderingQueued(float timePassed) {
    if (keyVector[0])
        cameraTranslationNode->translate(glm::vec3(0.0f, 0.0f, -moveSpeed * timePassed), TransformSpace::PARENT);
    else if (keyVector[2])
        cameraTranslationNode->translate(glm::vec3(0.0f, 0.0f, moveSpeed * timePassed), TransformSpace::PARENT);

    if (keyVector[1])
        cameraTranslationNode->translate(glm::vec3(-moveSpeed * timePassed, 0.0f, 0.0f), TransformSpace::PARENT);
    else if (keyVector[3])
        cameraTranslationNode->translate(glm::vec3(moveSpeed * timePassed, 0.0f, 0.0f), TransformSpace::PARENT);
}

void FreeCamera::onFrameEnd(float timePassed) {
}