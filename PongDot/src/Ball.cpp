#include "Ball.h"
#include "SceneNode.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"


Ball::Ball() :
    Entity("cube", "pointMaterial"),
    velocity(8.0f, 0.0f, 4.0f) {

    setAsCollider("ball");
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

    pos += velocity * timePassed;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);
}

//TODO: This does not work because ww should send the uniform on the SubEntity prerender, not the Entity. The bound shader will not be correct.
void Ball::preRender(Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::preRender(renderer, renderOperation);
    //auto color = glm::vec3(0.875f, 0.902f, 0.0f);
    //material->getGpuProgram().getUniformByName("mainColor")->setValue(reinterpret_cast<const void*>(&color));
}


