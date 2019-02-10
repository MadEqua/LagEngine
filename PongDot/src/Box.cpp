#include "Box.h"

#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Color.h"

#include "Scene.h"
#include "Ball.h"


Box::Box(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
             const glm::vec3 &position, const glm::vec3 &scale, 
             const Lag::Color &color1, const Lag::Color &color2,
             float trisPerLength, const glm::vec2 &maxPointSizeLimits, const glm::vec2 displacementStrengthLimits) :
    Entity("cube", "pointMaterial"),
    trisPerLength (trisPerLength),
    maxPointSizeLimits(maxPointSizeLimits),
    displacementStrengthLimits(displacementStrengthLimits),
    maxPointSize(maxPointSizeLimits.x),
    displacementStrength(displacementStrengthLimits.x) {

    setAsCollider("wall");

    originalColor1 = color1;
    originalColor2 = color2;
    this->color1 = originalColor1;
    this->color2 = originalColor2;

    Lag::SceneNode &planeNode = parentNode.createChildSceneNode(name);
    planeNode.setPosition(position);
    planeNode.setScale(scale);

    scene.addEntity(this);
    attachToSceneNode(planeNode);
}

void Box::onCollision(Entity &other, const Lag::IntersectionResult &result) {
    if(other.getColliderName() == "ball") {
        timeToAnimate = TIME_TO_ANIMATE;
        Ball &ball = static_cast<Ball&>(other);
        colorPassedByBall = ball.getColor();
    }
}

void Box::onFrameStart(float timePassed) {
    if(timeToAnimate > 0.0f) {
        timeToAnimate = glm::max(0.0f, timeToAnimate - timePassed);
        float ratio = timeToAnimate / TIME_TO_ANIMATE;

        displacementStrength = glm::mix(displacementStrengthLimits.x, displacementStrengthLimits.y, ratio);
        maxPointSize = glm::mix(maxPointSizeLimits.x, maxPointSizeLimits.y, ratio);

        color1 = glm::mix(originalColor1.toVec3(), colorPassedByBall.toVec3(), ratio);
        color2 = glm::mix(originalColor2.toVec3(), colorPassedByBall.toVec3(), ratio);

        maxPointSize = glm::mix(maxPointSizeLimits.x, maxPointSizeLimits.y, ratio);   
    }

    Entity::onFrameStart(timePassed);
}

void Box::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);

    Lag::uint32 color1 = this->color1.toIntABGR();
    Lag::uint32 color2 = this->color2.toIntABGR();

    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}