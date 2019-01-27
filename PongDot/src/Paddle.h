#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Entity.h"

class Ball;

class Paddle : public Lag::Entity {
public:
    Paddle(std::vector<Ball*> &ballsm, const glm::vec3 &normal, const glm::vec3 &tangent);

    void onFrameStart(float timePassed) override;

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    struct ClosestBall {
        Ball *ball;
        float distance;
    };

    glm::vec3 velocity;
    Lag::uint32 color1, color2;
    std::vector<Ball*> balls;

    glm::vec3 normal;
    glm::vec3 tangent;

    void updateAI();
    ClosestBall findClosestBallComingTowards();

    const float SPEED = 15.0f;
    const float DIST_THRESHOLD = 1.0f;
};