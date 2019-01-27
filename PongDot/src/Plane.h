#pragma once

#include "Entity.h"
#include "Color.h"

namespace Lag {
    class Scene;
    class SceneNode;
}

class Plane : public Lag::Entity {
public:
    Plane(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
          const glm::vec3 &position, const glm::vec3 &scale,
          const Lag::Color &color1, const Lag::Color &color2, float trisPerLength, float maxPointSize, float displacementStrength);

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    Lag::uint32 color1, color2;
    float trisPerLength; //Control tesselation
    float maxPointSize;
    float displacementStrength;
};