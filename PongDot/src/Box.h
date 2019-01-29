#pragma once

#include "Entity.h"
#include "Color.h"

namespace Lag {
    class Scene;
    class SceneNode;
}

class Box : public Lag::Entity {
public:
    Box(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
          const glm::vec3 &position, const glm::vec3 &scale,
          const Lag::Color &color1, const Lag::Color &color2, 
          float trisPerLength, const glm::vec2 &maxPointSizeLimits, const glm::vec2 displacementStrengthLimits);

    void onCollision(Entity &other) override;
    void onFrameStart(float timePassed) override;

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    Lag::Color originalColor1, originalColor2;
    Lag::Color color1, color2;

    float trisPerLength; //Control tesselation
    glm::vec2 maxPointSizeLimits;
    glm::vec2 displacementStrengthLimits;

    Lag::Color colorPassedByBall;

    float maxPointSize;
    float displacementStrength;

    const float TIME_TO_ANIMATE = 2.0f;
    float timeToAnimate;
};