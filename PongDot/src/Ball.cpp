#include "Ball.h"
#include "SceneNode.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Scene.h"
#include "PointLight.h"

#include <glm/gtc/random.hpp>


Ball::Ball(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name) :
    Entity("sphere", "ballMaterial"),
    velocity(10.0f, 0.0f, 12.0f),
    timeToDimElapsed(0.0f) {

    setAsCollider("ball");

    lightBaseColor = Lag::Color(glm::linearRand(0.1f, 1.0f), glm::linearRand(0.1f, 1.0f), glm::linearRand(0.1f, 1.0f));

    this->color = lightBaseColor.toIntABGR();

    sceneNode = &parentNode.createChildSceneNode(name);
    sceneNode->setPosition(glm::vec3(glm::linearRand(-20.0f, 20.0f), 0.75f, glm::linearRand(-20.0f, 20.0f)));
    sceneNode->setScale(glm::vec3(1.1f));

    scene.addEntity(this);
    attachToSceneNode(*sceneNode);

    light = &scene.createPointLight(lightBaseColor * LIGHT_INTENSITY, glm::vec3(1.0f, 0.1f, 0.1f));
    light->attachToSceneNode(*sceneNode);
}

void Ball::onCollision(Entity &other) {
    isColliding = true;
    timeToDimElapsed = TIME_TO_FLASH;
}

void Ball::onFrameStart(float timePassed) {
    glm::vec3 pos = getWorldPosition();
    
    if(isColliding) {
        isColliding = false;

        glm::vec3 n = glm::abs(pos.x) > glm::abs(pos.z) ? 
            glm::vec3(-glm::sign(pos.x), 0.0f, 0.0f) :
            glm::vec3(0.0f, 0.0f, -glm::sign(pos.z));

        velocity = glm::reflect(velocity, n);
    }

    glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), velocity));
    getParentSceneNode()->rotate(25.0f * glm::length(velocity) * timePassed, rotationAxis, Lag::TransformSpace::WORLD);

    getParentSceneNode()->translate(velocity * timePassed, Lag::TransformSpace::WORLD);

    if(timeToDimElapsed > 0.0f) {
        timeToDimElapsed = glm::max(0.0f, timeToDimElapsed - timePassed);

        Lag::Color color = lightBaseColor;
        float mix = glm::mix(1.0f, LIGHT_INTENSITY, timeToDimElapsed / TIME_TO_FLASH);
        light->setColor(color * mix);
    }

    Entity::onFrameStart(timePassed);
}

void Ball::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);
    material->getGpuProgram().getUniformByName("color")->setValue(reinterpret_cast<const void*>(&color));

    const float trisPerLength = 1.5f;
    const float maxPointSize = glm::mix(1.5f, 6.0f, (timeToDimElapsed / TIME_TO_FLASH));
    const float displacementStrength = glm::mix(0.1f, 0.4f, (timeToDimElapsed / TIME_TO_FLASH));

    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}