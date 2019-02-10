#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Entity.h"

class Ball;

namespace Lag {
    class Scene;
    class SceneNode;
}

class Paddle : public Lag::Entity {
public:
    Paddle(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name, 
           std::vector<Ball*> &balls, const glm::vec3 &position, const glm::vec3 &scale,
           const glm::vec3 &normal, const glm::vec3 &tangent);

    void onFrameStart(float timePassed) override;

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    struct ClosestBall {
        Ball *ball;
        float distance;
    };

    glm::vec3 acceleration;
    glm::vec3 velocity;
    Lag::uint32 color1, color2;
    std::vector<Ball*> balls;

    glm::vec3 normal;
    glm::vec3 tangent;

    void updateAI();
    ClosestBall findClosestBallComingTowards();

    const float ACCEL = 4.0f;
    const float FRICTION = 0.85f;
    const float BALL_CLOSE_ENOUGH_THRESHOLD = 1.0f;
};