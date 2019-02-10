#pragma once

#include "Scene.h"
#include "ICollisionListener.h"

namespace Lag {
    class FreeCamera;
    class InterpolatedCamera;
    class SceneNode;
}

class MainScene : public Lag::Scene, public Lag::ICollisionListener {
public:
    void onStart() override;
    void onEnd() override;
    void onInitializeViewports(Lag::RenderWindow &renderWindow) override;

    void onFrameStart(float timePassed) override;
    void onCollision(Lag::Entity &entity1, Lag::Entity &entity2, const Lag::IntersectionResult &result) override;

private:
    //Lag::FreeCamera *camera;
    Lag::InterpolatedCamera *camera;

    void initBallsAndPaddles(Lag::SceneNode &parentNode);
    void initBoard(Lag::SceneNode &parentNode);
    void initGround(Lag::SceneNode &parentNode);

    const float BOARD_SIZE = 50.0f;
    const float HALF_BOARD_SIZE = BOARD_SIZE * 0.5f;
    const float QUARTER_BOARD_SIZE = HALF_BOARD_SIZE * 0.5f;
};
