#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

class Paddle : public Lag::Entity {
public:
    Paddle();

    void onFrameStart(float timePassed) override;

    void onKeyPress(int key, int modifier) override;
    void onKeyRelease(int key, int modifier) override;

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    glm::vec3 velocity;
    Lag::uint32 color1, color2;
};