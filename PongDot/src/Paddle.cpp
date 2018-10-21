#include "Paddle.h"
#include "SceneNode.h"
#include "Keys.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"


Paddle::Paddle() :
    Entity("cube", "pointMaterial"),
    velocity(0.0f) {

    setAsCollider("paddle");
}

void Paddle::onFrameStart(float timePassed) {
    glm::vec3 pos = getWorldPosition();
    pos += velocity * timePassed;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);
}

void Paddle::onKeyPress(int key, int modifier)  {
    if(key == KEY_UP) velocity.z -= 10.0f;
    if(key == KEY_DOWN) velocity.z += 10.0f;
}
void Paddle::onKeyRelease(int key, int modifier) {
    if(key == KEY_UP) velocity.z += 10.0f;
    if(key == KEY_DOWN) velocity.z -= 10.0f;
}

void Paddle::preRender(Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::preRender(renderer, renderOperation);
    //auto color = glm::vec3(0.08f, 0.06f, 0.9f);
    //material->getGpuProgram().getUniformByName("mainColor")->setValue(reinterpret_cast<const void*>(&color));
}