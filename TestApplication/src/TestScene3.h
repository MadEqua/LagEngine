#pragma once

#include "Scene.h"

namespace Lag {
    class FreeCamera;
}

class TestScene3 : public Lag::Scene {
public:
    void onStart() override;
    void onEnd() override;
    void onInitializeViewports(Lag::RenderWindow &renderWindow) override;

    void onFrameStart(float timePassed) override;

private:
    Lag::FreeCamera *camera;
};
