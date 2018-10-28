#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

namespace Lag {
    struct RenderOperation;
};

class Ball : public Lag::Entity {
public:
    Ball();

    void onCollision(Entity &other) override;
    void onFrameStart(float timePassed) override;

    void preRender(Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    bool isColliding;
    glm::vec3 velocity;
};