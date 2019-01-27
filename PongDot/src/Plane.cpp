#include "Plane.h"

#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Color.h"

#include "Scene.h"


Plane::Plane(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
             const glm::vec3 &position, const glm::vec3 &scale, 
             const Lag::Color &color1, const Lag::Color &color2, float trisPerLength, float maxPointSize, float displacementStrength) :
    Entity("plane", "pointMaterial"),
    trisPerLength (trisPerLength),
    maxPointSize(maxPointSize),
    displacementStrength(displacementStrength) {

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();

    Lag::SceneNode &planeNode = parentNode.createChildSceneNode(name);
    planeNode.setPosition(position);
    planeNode.setScale(scale);

    scene.addEntity(this);
    attachToSceneNode(planeNode);
}

void Plane::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);

    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}