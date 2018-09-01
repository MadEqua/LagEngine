#include "TestScene2.h"

#include "Root.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "PerspectiveCamera.h"
#include "FreeCamera.h"
#include "Entity.h"
#include "MeshRepository.h"

void TestScene2::onStart() {
    Lag::Root &root = Lag::Root::getInstance();
    auto &meshRepo = root.getMeshRepository();

    root.getRenderer().setPointSizeFromGpuProgramEnabled(true);
    root.getRenderer().setClearColor(Lag::Color(0.1f));

    Lag::SceneNode &rootNode = getSceneGraph().getRootSceneNode();

    Lag::SceneNode &boardNode = rootNode.createChildSceneNode("board");
    Lag::SceneNode &boardBaseNode = boardNode.createChildSceneNode("boardBase");
    Lag::SceneNode &boardLeftNode = boardNode.createChildSceneNode("boardLeft");
    Lag::SceneNode &boardRightNode = boardNode.createChildSceneNode("boardRight");
    Lag::SceneNode &boardTopNode = boardNode.createChildSceneNode("boardTop");
    Lag::SceneNode &boardBottomNode = boardNode.createChildSceneNode("boardBottom");

    const float SIZE = 50.0f;
    const float HALF_SIZE = SIZE / 2.0f;

    boardBaseNode.setScale(glm::vec3(SIZE, 1.0f, SIZE));

    boardLeftNode.setPosition(glm::vec3(-HALF_SIZE, 0.0f, 0.0f));
    boardLeftNode.setScale(glm::vec3(1.0f, 2.0f, SIZE));

    boardRightNode.setPosition(glm::vec3(HALF_SIZE, 0.0f, 0.0f));
    boardRightNode.setScale(glm::vec3(1.0f, 2.0f, SIZE));

    boardTopNode.setPosition(glm::vec3(0.0f, 0.0f, -HALF_SIZE));
    boardTopNode.setScale(glm::vec3(SIZE, 2.0f, 1.0f));

    boardBottomNode.setPosition(glm::vec3(0.0f, 0.0f, HALF_SIZE));
    boardBottomNode.setScale(glm::vec3(SIZE, 2.0f, 1.0f));

    Lag::Entity *basePlane = createEntity(meshRepo.getPlaneXZ(), "pointMaterial");
    basePlane->attachToSceneNode(boardBaseNode);

    Lag::Entity *leftCube = createEntity(meshRepo.getCube(), "pointMaterial");
    leftCube->attachToSceneNode(boardLeftNode);

    Lag::Entity *rightCube = createEntity(meshRepo.getCube(), "pointMaterial");
    rightCube->attachToSceneNode(boardRightNode);

    Lag::Entity *topCube = createEntity(meshRepo.getCube(), "pointMaterial");
    topCube->attachToSceneNode(boardTopNode);

    Lag::Entity *bottomCube = createEntity(meshRepo.getCube(), "pointMaterial");
    bottomCube->attachToSceneNode(boardBottomNode);
}

void TestScene2::onEnd() {
    delete camera;
}

void TestScene2::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));

    renderWindow.createViewport(camera->getCamera());
}