#include "Paddle.h"

#include "SceneNode.h"
#include "Keys.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"

#include "Ball.h"
#include "Scene.h"

#include <glm/glm.hpp>

Paddle::Paddle(Lag::Scene &scene, Lag::SceneNode &parentNode, const std::string &name,
               std::vector<Ball*> &balls, const glm::vec3 &position, const glm::vec3 &scale,
               const glm::vec3 &normal, const glm::vec3 &tangent) :
    Entity("cube", "pointMaterial"),
    velocity(0.0f),
    acceleration(0.0f),
    balls(balls),
    normal(normal),
    tangent(tangent) {

    setAsCollider("paddle");

    Lag::Color color1(0.2f, 0.06f, 0.08f);
    Lag::Color color2(0.7f, 0.04f, 0.01f);

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();

    Lag::SceneNode &paddleNode = parentNode.createChildSceneNode(name);
    paddleNode.setPosition(position);
    paddleNode.setScale(scale);

    scene.addEntity(this);
    attachToSceneNode(paddleNode);
}

void Paddle::onFrameStart(float timePassed) {
    updateAI();

    velocity += acceleration;
    velocity *= FRICTION;

    glm::vec3 pos = getWorldPosition();
    pos += velocity * timePassed;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);

    Entity::onFrameStart(timePassed);
}

void Paddle::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);
    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    const float trisPerLength = 15.0f;
    const float maxPointSize = 4.0f;
    const float displacementStrength = 0.05f;
    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
    material->getGpuProgram().getUniformByName("maxPointSize")->setValue(reinterpret_cast<const void*>(&maxPointSize));
    material->getGpuProgram().getUniformByName("displacementStrength")->setValue(reinterpret_cast<const void*>(&displacementStrength));
}

void Paddle::updateAI() {
    ClosestBall closestBallComingTowards = findClosestBallComingTowards();
    acceleration = glm::vec3(0.0f);

    if(closestBallComingTowards.ball != nullptr) {
        glm::vec3 closestBallPos = closestBallComingTowards.ball->getWorldPosition();
        glm::vec3 myPos = getWorldPosition();

        glm::vec3 paddleToBallVec = closestBallPos - myPos;
        float projOnTangent = glm::dot(paddleToBallVec, tangent);

        if(std::fabs(projOnTangent) > BALL_CLOSE_ENOUGH_THRESHOLD) {
            acceleration = tangent * ACCEL * glm::sign(projOnTangent);
        }
    }
}

Paddle::ClosestBall Paddle::findClosestBallComingTowards() {
    ClosestBall closestBall;
    
    closestBall.distance = Lag::MAX_FLOAT;
    closestBall.ball = nullptr;
    
    for(auto ball : balls) {
        glm::vec3 myPos = getWorldPosition();
        glm::vec3 ballPos = ball->getWorldPosition();
        float dist = glm::distance(ballPos, myPos);

        if(dist < closestBall.distance && glm::dot(normal, ball->getVelocity()) < 0.0f) {
            closestBall.distance = dist;
            closestBall.ball = ball;
        }
    }
    return closestBall;
}
