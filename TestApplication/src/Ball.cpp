#include "Ball.h"
#include "SceneNode.h"

Ball::Ball() :
    Entity("cube", "pointMaterial"),
    velocity(0.2f, 0.0f, 0.1f) {

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

    pos += velocity;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);
}
