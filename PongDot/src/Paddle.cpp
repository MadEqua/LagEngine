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

    Lag::Color color1(0.2f, 0.06f, 0.08f);
    Lag::Color color2(0.1f, 0.04f, 0.01f);

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();
}

void Paddle::onFrameStart(float timePassed) {
    glm::vec3 pos = getWorldPosition();
    pos += velocity * timePassed;
    getParentSceneNode()->setPosition(pos, Lag::TransformSpace::WORLD);

    Entity::onFrameStart(timePassed);
}

void Paddle::onKeyPress(int key, int modifier)  {
    if(key == KEY_UP) velocity.z -= 10.0f;
    if(key == KEY_DOWN) velocity.z += 10.0f;
}
void Paddle::onKeyRelease(int key, int modifier) {
    if(key == KEY_UP) velocity.z += 10.0f;
    if(key == KEY_DOWN) velocity.z -= 10.0f;
}

void Paddle::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);
    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));

    const float trisPerLength = 1.5f;
    material->getGpuProgram().getUniformByName("trisPerLength")->setValue(reinterpret_cast<const void*>(&trisPerLength));
}