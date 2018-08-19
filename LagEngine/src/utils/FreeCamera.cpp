#include "FreeCamera.h"

#include "Renderer.h"
#include "Scene.h"
#include "InputManager.h"
#include "RenderWindow.h"

using namespace Lag;

FreeCamera::FreeCamera(Scene &scene, float fovy, float nearPlane, float farPlane, float moveSpeed) :
        moveSpeed(moveSpeed) {

    keyVector[0] = keyVector[1] = keyVector[2] = keyVector[3] = false;
    lastCursor[0] = lastCursor[1] = -1;

    Root &root = Root::getInstance();

    SceneNode &rootNode = scene.getSceneGraph().getRootSceneNode();
    sceneNode = &rootNode.createChildSceneNode("cameraSceneNode");

    camera = &scene.createPerspectiveCamera(1.0f, fovy, nearPlane, farPlane);
    camera->attachToSceneNode(*sceneNode);

    root.getRenderer().registerObserver(*this);
    root.getInputManager().registerObserver(static_cast<Lag::IKeyboardListener &>(*this));
    root.getInputManager().registerObserver(static_cast<Lag::ICursorListener &>(*this));

    renderWindow = root.getRenderTargetManager().getRenderWindow();
    lastFrameVirtualCursor = renderWindow->isVirtualCursorEnabled();
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
    if(renderWindow->isVirtualCursorEnabled() != lastFrameVirtualCursor) {
        lastFrameVirtualCursor = renderWindow->isVirtualCursorEnabled();
        lastCursor[0] = -1;
        lastCursor[1] = -1;
    }

    int dx, dy;

    if (lastCursor[0] != -1) {
        dx = lastCursor[0] - x;
        sceneNode->yaw(static_cast<float>(dx) * 0.1f, TransformSpace::WORLD);
    }

    if (lastCursor[1] != -1) {
        dy = lastCursor[1] - y;
        sceneNode->pitch(static_cast<float>(dy) * 0.1f, TransformSpace::LOCAL);
    }

    lastCursor[0] = x;
    lastCursor[1] = y;
}

void FreeCamera::onButtonPressed(int x, int y, int button, int modifiers) {
}

void FreeCamera::onButtonReleased(int x, int y, int button, int modifiers) {
}

void FreeCamera::onFrameStart(float timePassed) {
    if (keyVector[0])
        sceneNode->translate(glm::vec3(0.0f, 0.0f, -moveSpeed * timePassed), TransformSpace::LOCAL);
    else if (keyVector[2])
        sceneNode->translate(glm::vec3(0.0f, 0.0f, moveSpeed * timePassed), TransformSpace::LOCAL);

    if (keyVector[1])
        sceneNode->translate(glm::vec3(-moveSpeed * timePassed, 0.0f, 0.0f), TransformSpace::LOCAL);
    else if (keyVector[3])
        sceneNode->translate(glm::vec3(moveSpeed * timePassed, 0.0f, 0.0f), TransformSpace::LOCAL);
}

void FreeCamera::onFrameRenderingQueued(float timePassed) {
}

void FreeCamera::onFrameEnd(float timePassed) {
}