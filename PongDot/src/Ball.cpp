#include "Ball.h"
#include "SceneNode.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Scene.h"
#include "PointLight.h"
#include "Utils.h"

#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

#include "Root.h"
#include "InputManager.h"


Ball::Ball(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name) :
    Entity("sphere", "ballMaterial"),
    velocity(9.0f, 0.0f, 10.0f),
    timeToFlash(0.0f) {

    setAsCollider("ball");

    //glm::vec3 rgb(glm::linearRand(0.5f, 1.0f), glm::linearRand(0.5f, 1.0f), glm::linearRand(0.5f, 1.0f));
    glm::vec3 hsv(glm::linearRand(0.0f, 360.0f), glm::linearRand(0.8f, 0.99f), glm::linearRand(0.8f, 0.99f));
    color = Lag::Color(Lag::Utils::HSVtoRGB(hsv));

    Lag::SceneNode &sceneNode = parentNode.createChildSceneNode(name);
    sceneNode.setPosition(glm::vec3(glm::linearRand(-20.0f, 20.0f), 0.75f, glm::linearRand(-20.0f, 20.0f)));
    sceneNode.setScale(glm::vec3(1.1f));

    scene.addEntity(this);
    attachToSceneNode(sceneNode);

    light = &scene.createPointLight(lightBaseColor * LIGHT_INTENSITY, glm::vec3(0.0f, 0.0f, .08f));
    light->attachToSceneNode(sceneNode);
}

void Ball::onCollision(Entity &other, const Lag::IntersectionResult &result) {
    intersectionResult = result;
    timeToFlash = TIME_TO_FLASH;
}

void Ball::onFrameStart(float timePassed) {
    glm::vec3 pos = getWorldPosition();
    
    if(intersectionResult.intersects) {
        intersectionResult.intersects = false;
        intersectionResult.penetration.y = 0.0f;

        glm::vec3 pen = -intersectionResult.penetration;
        parentSceneNode->translate(pen, Lag::TransformSpace::WORLD);

        velocity = glm::reflect(velocity, glm::normalize(pen));
    }

    glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), velocity));
    getParentSceneNode()->rotate(25.0f * glm::length(velocity) * timePassed, rotationAxis, Lag::TransformSpace::WORLD);

    getParentSceneNode()->translate(velocity * timePassed, Lag::TransformSpace::WORLD);

    if(timeToFlash > 0.0f) {
        timeToFlash = glm::max(0.0f, timeToFlash - timePassed);

        Lag::Color color = lightBaseColor;
        float mix = glm::mix(1.0f, LIGHT_INTENSITY, timeToFlash / TIME_TO_FLASH);
        light->setColor(color * mix);
    }

    Entity::onFrameStart(timePassed);
}

void Ball::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);

    const Lag::uint32 color = this->color.toIntABGR();
    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color));

    const float trisPerLength = 1.0f;
    const float maxPointSize = glm::mix(1.5f, 6.0f, (timeToFlash / TIME_TO_FLASH));
    const float displacementStrength = glm::mix(0.1f, 0.4f, (timeToFlash / TIME_TO_FLASH));

    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}