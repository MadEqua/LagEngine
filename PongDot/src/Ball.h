#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

class Ball : public Lag::Entity {
public:
    Ball();

    void onCollision(Entity &other) override;
    void onFrameStart(float timePassed) override;

    inline const glm::vec3& getVelocity() const { return velocity; }

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    bool isColliding;
    glm::vec3 velocity;
    Lag::uint32 color1, color2;
};