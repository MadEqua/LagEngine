#pragma once

#include "Scene.h"
#include "ICollisionListener.h"

namespace Lag {
    class FreeCamera;
    class SceneNode;
}

class TestScene2 : public Lag::Scene, public Lag::ICollisionListener {
public:
    void onStart() override;
    void onEnd() override;
    void onInitializeViewports(Lag::RenderWindow &renderWindow) override;

    void onFrameStart(float timePassed) override;
    void onCollision(Lag::Entity &entity1, Lag::Entity &entity2) override;

private:
    Lag::FreeCamera *camera;

    void initBalls(Lag::SceneNode &parentNode, int count);
};
