#pragma once

#include "Entity.h"

#include <glm/glm.hpp>

namespace Lag {
    class Scene;
    class SceneNode;
}

class Star : public Lag::Entity {
public:
    Star(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
         const glm::vec3 &position, const glm::vec3 &scale,
         Lag::SceneNode &cameraSceneNode);

    void onFrameStart(float timePassed) override;

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    Lag::uint32 color1, color2;
    Lag::SceneNode &cameraSceneNode;
};