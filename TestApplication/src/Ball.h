#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

class Ball : public Lag::Entity {
public:
    Ball();

    void onCollision(Entity &other) override;
    void onFrameStart(float timePassed) override;

private:
    bool isColliding;
    glm::vec3 velocity;
};