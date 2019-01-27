#include "Ball.h"
#include "SceneNode.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"


Ball::Ball() :
    Entity("sphere", "ballMaterial"),
    velocity(10.0f, 0.0f, 12.0f) {

    setAsCollider("ball");

    Lag::Color color1(0.8f, 0.06f, 0.08f);
    Lag::Color color2(0.4f, 0.04f, 0.01f);

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();
}

void Ball::onCollision(Entity &other) {
    isColliding = true;
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

    Entity::onFrameStart(timePassed);
}

void Ball::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);
    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    const float trisPerLength = 1.5f;
    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
}