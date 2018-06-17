#include "TestScene2.h"

#include "Root.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "FreeCamera.h"
#include "SceneNode.h"
#include "Entity.h"

void TestScene2::onStart() {
    Lag::Root &root = Lag::Root::getInstance();

    root.getRenderer().setPointSizeFromGpuProgramEnabled(true);
    root.getRenderer().setClearColor(Lag::Color(0.0f));

    Lag::SceneNode &mainNode = getSceneGraph().getRootSceneNode().createChildSceneNode("main");
    mainNode.setScale(glm::vec3(20.0f));

    Lag::Entity *ent = createEntity("bunny", "objectMaterial");
    ent->attachToSceneNode(mainNode);
}

void TestScene2::onEnd() {
    delete camera;
}

void TestScene2::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));

    renderWindow.createViewport(camera->getCamera());
}