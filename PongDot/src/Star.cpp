#include "Star.h"

#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Color.h"

#include "Scene.h"

#include <glm/gtc/random.hpp>


Star::Star(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
           const glm::vec3 &position, const glm::vec3 &scale,
           Lag::SceneNode &cameraSceneNode) :
    Entity("star", "ballMaterial"),
    cameraSceneNode(cameraSceneNode) {

    Lag::Color color1(1.0f, 1.0f, 0.0f);
    Lag::Color color2(1.0f, 0.9f, 0.1f);
    
    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();

    Lag::SceneNode &planeNode = parentNode.createChildSceneNode(name);
    planeNode.setPosition(position);
    planeNode.setScale(scale);

    scene.addEntity(this);
    attachToSceneNode(planeNode);
}

void Star::onFrameStart(float timePassed) {
    getParentSceneNode()->lookAt(getWorldPosition(),
                                 cameraSceneNode.getPositionWorldSpace(),
                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                 Lag::TransformSpace::WORLD);
}


void Star::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);

    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    const float trisPerLength = 0.5f;
    const float maxPointSize = 3.0f;
    const float displacementStrength = 0.9f;
    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}