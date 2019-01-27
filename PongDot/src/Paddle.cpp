#include "Paddle.h"

#include "SceneNode.h"
#include "Keys.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"

#include "Ball.h"
#include <glm/glm.hpp>

Paddle::Paddle(std::vector<Ball*> &balls, const glm::vec3 &normal, const glm::vec3 &tangent) :
    Entity("cube", "pointMaterial"),
    velocity(0.0f),
    balls(balls),
    normal(normal),
    tangent(tangent) {

    setAsCollider("paddle");

    Lag::Color color1(0.2f, 0.06f, 0.08f);
    Lag::Color color2(0.1f, 0.04f, 0.01f);

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();
}

void Paddle::onFrameStart(float timePassed) {
    updateAI();

    glm::vec3 pos = getWorldPosition();
    pos += velocity * timePassed;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);

    Entity::onFrameStart(timePassed);
}

void Paddle::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);
    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    const float trisPerLength = 2.0f;
    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
}

void Paddle::updateAI() {
    ClosestBall closestBallComingTowards = findClosestBallComingTowards();
    velocity = glm::vec3(0.0f);

    if(closestBallComingTowards.ball != nullptr) {
        glm::vec3 closestBallPos = closestBallComingTowards.ball->getWorldPosition();
        glm::vec3 myPos = getWorldPosition();

        glm::vec3 paddleToBallVec = closestBallPos - myPos;
        float projOnTangent = glm::dot(paddleToBallVec, tangent);

        if(std::fabs(projOnTangent) > DIST_THRESHOLD) {
            velocity = tangent * SPEED * glm::sign(projOnTangent);
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
