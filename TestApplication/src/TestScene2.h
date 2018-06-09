#pragma once

#include "Scene.h"

namespace Lag {
    class FreeCamera;
}

class TestScene2 : public Lag::Scene {
public:
    void onStart() override;
    void onEnd() override;
    void onInitializeViewports(Lag::RenderWindow &renderWindow) override;

private:
    Lag::FreeCamera *camera;
};
