#pragma once

#include "PerspectiveCamera.h"

#include "IKeyboardListener.h"
#include "ICursorListener.h"
#include "IFrameListener.h"


namespace Lag {
    class Scene;
    class RenderWindow;
    class PerspectiveCamera;
    class SceneNode;

    /*
    * Utility class for a classic free movement camera using WASD and mouse.
    */
    class FreeCamera : public IKeyboardListener, public ICursorListener, public IFrameListener {

    public:
        FreeCamera(Scene &scene, float fovy, float nearPlane, float farPlane, float moveSpeed);
        ~FreeCamera() override;

        inline Camera& getCamera() const { return *camera; }

        void onKeyPress(int key, int modifier) override;
        void onKeyRelease(int key, int modifier) override;
        void onKeyRepeat(int key, int modifier) override;

        void onCursorMove(int x, int y) override;
        void onButtonPressed(int x, int y, int button, int modifiers) override;
        void onButtonReleased(int x, int y, int button, int modifiers) override;

        void onFrameStart(float timePassed) override;
        void onFrameRenderingQueued(float timePassed) override;
        void onFrameEnd(float timePassed) override;

    protected:
        float moveSpeed;
        bool keyVector[5]; //wasd, shift
        int lastCursor[2]; //xy

        PerspectiveCamera *camera;
        SceneNode *sceneNode;

        bool lastFrameVirtualCursor;
        RenderWindow *renderWindow;

        const float SHIFT_SPEED_MOD = 5.0f;
    };
}