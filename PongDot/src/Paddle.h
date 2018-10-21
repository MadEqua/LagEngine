#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

namespace Lag {
    class RenderOperation;
};


class Paddle : public Lag::Entity {
public:
    Paddle();

    void onFrameStart(float timePassed) override;

    void onKeyPress(int key, int modifier) override;
    void onKeyRelease(int key, int modifier) override;

    void preRender(Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    glm::vec3 velocity;
};