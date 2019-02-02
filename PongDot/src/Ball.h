#pragma once

#include <glm/glm.hpp>

#include "Entity.h"
#include "Color.h"

namespace Lag {
    class Scene;
    class SceneNode;
    class Light;
}

class Ball : public Lag::Entity {
public:
    Ball(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name);

    void onCollision(Entity &other) override;
    void onFrameStart(float timePassed) override;

    inline const glm::vec3& getVelocity() const { return velocity; }
    inline const Lag::Color &getColor() const { return color; }

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    bool isColliding;
    glm::vec3 velocity;
    Lag::Color color;

    Lag::Light *light;
    Lag::Color lightBaseColor;

    const float TIME_TO_FLASH = 0.33f;
    const float LIGHT_INTENSITY = 10.0f;

    float timeToFlash;
};