#include "TestScene2.h"

#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>

#include "Root.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "PerspectiveCamera.h"
#include "FreeCamera.h"
#include "Entity.h"
#include "MeshManager.h"
#include "PointLight.h"
#include "CollisionManager.h"
#include "Ball.h"



void TestScene2::onStart() {
    Lag::Root &root = Lag::Root::getInstance();
    auto &meshManager = root.getMeshManager();

    root.getRenderer().setPointSizeFromGpuProgramEnabled(true);
    root.getRenderer().setClearColor(Lag::Color(0.001f));

    Lag::SceneNode &rootNode = getSceneGraph().getRootSceneNode();

    Lag::SceneNode &boardNode = rootNode.createChildSceneNode("board");

    //TODO: instead of many planes, we should use a single tessellated plane (the GPU will further tesselate it)
    //To achieve the desired result with a single plane we reach the GPU tessellation limit too easily.
    Lag::SceneNode &boardBaseNode1 = boardNode.createChildSceneNode("boardBase1");
    Lag::SceneNode &boardBaseNode2 = boardNode.createChildSceneNode("boardBase2");
    Lag::SceneNode &boardBaseNode3 = boardNode.createChildSceneNode("boardBase3");
    Lag::SceneNode &boardBaseNode4 = boardNode.createChildSceneNode("boardBase4");

    Lag::SceneNode &boardLeftNode = boardNode.createChildSceneNode("boardLeft");
    Lag::SceneNode &boardRightNode = boardNode.createChildSceneNode("boardRight");
    Lag::SceneNode &boardTopNode = boardNode.createChildSceneNode("boardTop");
    Lag::SceneNode &boardBottomNode = boardNode.createChildSceneNode("boardBottom");

    Lag::SceneNode &tower1Node = boardNode.createChildSceneNode("tower1");
    Lag::SceneNode &tower2Node = boardNode.createChildSceneNode("tower2");
    Lag::SceneNode &tower3Node = boardNode.createChildSceneNode("tower3");
    Lag::SceneNode &tower4Node = boardNode.createChildSceneNode("tower4");
    Lag::SceneNode &light1Node = tower1Node.createChildSceneNode("light1");
    Lag::SceneNode &light2Node = tower2Node.createChildSceneNode("light2");
    Lag::SceneNode &light3Node = tower3Node.createChildSceneNode("light3");
    Lag::SceneNode &light4Node = tower4Node.createChildSceneNode("light4");


    const float SIZE = 50.0f;
    const float HALF_SIZE = SIZE * 0.5f;
    const float QUARTER_SIZE = HALF_SIZE * 0.5f;

    const float WALL_HEIGHT = 2.0f;
    const float HALF_WALL_HEIGHT = WALL_HEIGHT * 0.5f;

    const float TOWER_HEIGHT = WALL_HEIGHT * 5.0f;
    const float HALF_TOWER_HEIGHT = TOWER_HEIGHT * 0.5f;
    
    boardBaseNode1.setScale(glm::vec3(HALF_SIZE, 1.0f, HALF_SIZE));
    boardBaseNode1.setPosition(glm::vec3(-QUARTER_SIZE, 0.0f, -QUARTER_SIZE));

    boardBaseNode2.setScale(glm::vec3(HALF_SIZE, 1.0f, HALF_SIZE));
    boardBaseNode2.setPosition(glm::vec3(QUARTER_SIZE, 0.0f, -QUARTER_SIZE));

    boardBaseNode3.setScale(glm::vec3(HALF_SIZE, 1.0f, HALF_SIZE));
    boardBaseNode3.setPosition(glm::vec3(-QUARTER_SIZE, 0.0f, QUARTER_SIZE));

    boardBaseNode4.setScale(glm::vec3(HALF_SIZE, 1.0f, HALF_SIZE));
    boardBaseNode4.setPosition(glm::vec3(QUARTER_SIZE, 0.0f, QUARTER_SIZE));
    
    boardLeftNode.setPosition(glm::vec3(-HALF_SIZE, HALF_WALL_HEIGHT, 0.0f));
    boardLeftNode.setScale(glm::vec3(1.0f, WALL_HEIGHT, SIZE));

    boardRightNode.setPosition(glm::vec3(HALF_SIZE, HALF_WALL_HEIGHT, 0.0f));
    boardRightNode.setScale(glm::vec3(1.0f, WALL_HEIGHT, SIZE));

    boardTopNode.setPosition(glm::vec3(0.0f, HALF_WALL_HEIGHT, -HALF_SIZE));
    boardTopNode.setScale(glm::vec3(SIZE, WALL_HEIGHT, 1.0f));

    boardBottomNode.setPosition(glm::vec3(0.0f, HALF_WALL_HEIGHT, HALF_SIZE));
    boardBottomNode.setScale(glm::vec3(SIZE, WALL_HEIGHT, 1.0f));

    tower1Node.setPosition(glm::vec3(-HALF_SIZE, HALF_TOWER_HEIGHT, -HALF_SIZE));
    tower1Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower2Node.setPosition(glm::vec3(HALF_SIZE, HALF_TOWER_HEIGHT, HALF_SIZE));
    tower2Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower3Node.setPosition(glm::vec3(HALF_SIZE, HALF_TOWER_HEIGHT, -HALF_SIZE));
    tower3Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower4Node.setPosition(glm::vec3(-HALF_SIZE, HALF_TOWER_HEIGHT, HALF_SIZE));
    tower4Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));

    light1Node.setInheritScale(false);
    light1Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light2Node.setInheritScale(false);
    light2Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light3Node.setInheritScale(false);
    light3Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light4Node.setInheritScale(false);
    light4Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);


    Lag::Entity *basePlane1 = createEntity(meshManager.getPlaneXZ(), "pointMaterial");
    basePlane1->attachToSceneNode(boardBaseNode1);

    Lag::Entity *basePlane2 = createEntity(meshManager.getPlaneXZ(), "pointMaterial");
    basePlane2->attachToSceneNode(boardBaseNode2);

    Lag::Entity *basePlane3 = createEntity(meshManager.getPlaneXZ(), "pointMaterial");
    basePlane3->attachToSceneNode(boardBaseNode3);

    Lag::Entity *basePlane4 = createEntity(meshManager.getPlaneXZ(), "pointMaterial");
    basePlane4->attachToSceneNode(boardBaseNode4);

    Lag::Entity *leftCube = createEntity("cube", "pointMaterial");
    leftCube->attachToSceneNode(boardLeftNode);
    leftCube->setAsCollider("wall");

    Lag::Entity *rightCube = createEntity("cube", "pointMaterial");
    rightCube->attachToSceneNode(boardRightNode);
    rightCube->setAsCollider("wall");

    Lag::Entity *topCube = createEntity("cube", "pointMaterial");
    topCube->attachToSceneNode(boardTopNode);
    topCube->setAsCollider("wall");

    Lag::Entity *bottomCube = createEntity("cube", "pointMaterial");
    bottomCube->attachToSceneNode(boardBottomNode);
    bottomCube->setAsCollider("wall");

    Lag::Entity *tower1Cube = createEntity("cube", "pointMaterial");
    tower1Cube->attachToSceneNode(tower1Node);

    Lag::Entity *tower2Cube = createEntity("cube", "pointMaterial");
    tower2Cube->attachToSceneNode(tower2Node);

    Lag::Entity *tower3Cube = createEntity("cube", "pointMaterial");
    tower3Cube->attachToSceneNode(tower3Node);

    Lag::Entity *tower4Cube = createEntity("cube", "pointMaterial");
    tower4Cube->attachToSceneNode(tower4Node);


    const glm::vec3 TOWER_LIGHT_ATTENUATIONS = glm::vec3(1.0f, 0.03f, 0.03f);

    Lag::PointLight &light1 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light1.attachToSceneNode(light1Node);

    Lag::PointLight &light2 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light2.attachToSceneNode(light2Node);

    Lag::PointLight &light3 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light3.attachToSceneNode(light3Node);

    Lag::PointLight &light4 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light4.attachToSceneNode(light4Node);

    initBalls(rootNode, 4);

    Lag::Root::getInstance().getCollisionManager().registerObserver(*this);
}

void TestScene2::onEnd() {
    delete camera;
}

void TestScene2::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));
    renderWindow.createViewport(camera->getCamera());
}

void TestScene2::onFrameStart(float timePassed) {
    Lag::Root::getInstance().getCollisionManager().checkCollisions();
    Scene::onFrameStart(timePassed);
}

void TestScene2::onCollision(Lag::Entity &entity1, Lag::Entity &entity2) {
}

void TestScene2::initBalls(Lag::SceneNode &parentNode, int count) {
    for(int i = 0; i < count; ++i) {
        Lag::SceneNode &ballNode = parentNode.createChildSceneNode("ball" + i);

        ballNode.setPosition(glm::vec3(glm::linearRand(-20.0f, 20.0f), 0.5f, glm::linearRand(-20.0f, 20.0f)));

        Ball *ball = new Ball();
        addEntity(ball);
        ball->attachToSceneNode(ballNode);

        Lag::PointLight &ballLight = createPointLight(Lag::Color(10.0f), glm::vec3(1.0f, 0.1f, 0.1f));
        ballLight.attachToSceneNode(ballNode);
    }
}
