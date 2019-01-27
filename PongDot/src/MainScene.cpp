#include "MainScene.h"

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
#include "Paddle.h"
#include "Plane.h"


void MainScene::onStart() {
    Lag::Root &root = Lag::Root::getInstance();
    auto &meshManager = root.getMeshManager();

    root.getRenderer().setPointSizeFromGpuProgramEnabled(true);
    root.getRenderer().setClearColor(Lag::Color(0.001f));

    Lag::SceneNode &rootNode = getSceneGraph().getRootSceneNode();

    initBoard(rootNode);
    initBalls(rootNode, 4);
    initPaddles(rootNode);
    initGround(rootNode);

    Lag::Root::getInstance().getCollisionManager().registerObserver(*this);
}

void MainScene::onEnd() {
    delete camera;
}

void MainScene::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));
    renderWindow.createViewport(camera->getCamera());
}

void MainScene::onFrameStart(float timePassed) {
    Lag::Root::getInstance().getCollisionManager().checkCollisions();
    Scene::onFrameStart(timePassed);
}

void MainScene::onCollision(Lag::Entity &entity1, Lag::Entity &entity2) {
}

void MainScene::initBalls(Lag::SceneNode &parentNode, int count) {
    for(int i = 0; i < count; ++i) {
        Lag::SceneNode &ballNode = parentNode.createChildSceneNode("ball" + i);

        ballNode.setPosition(glm::vec3(glm::linearRand(-20.0f, 20.0f), 0.5f, glm::linearRand(-20.0f, 20.0f)));

        Ball *ball = new Ball();
        addEntity(ball);
        ball->attachToSceneNode(ballNode);

        Lag::PointLight &ballLight = createPointLight(Lag::Color(15.0f), glm::vec3(1.0f, 0.1f, 0.1f));
        ballLight.attachToSceneNode(ballNode);
    }
}

void MainScene::initBoard(Lag::SceneNode &parentNode) {
    Lag::SceneNode &boardBaseNode = parentNode.createChildSceneNode("boardBase");

    Lag::SceneNode &boardLeftNode = parentNode.createChildSceneNode("boardLeft");
    Lag::SceneNode &boardRightNode = parentNode.createChildSceneNode("boardRight");
    Lag::SceneNode &boardTopNode = parentNode.createChildSceneNode("boardTop");
    Lag::SceneNode &boardBottomNode = parentNode.createChildSceneNode("boardBottom");

    Lag::SceneNode &tower1Node = parentNode.createChildSceneNode("tower1");
    Lag::SceneNode &tower2Node = parentNode.createChildSceneNode("tower2");
    Lag::SceneNode &tower3Node = parentNode.createChildSceneNode("tower3");
    Lag::SceneNode &tower4Node = parentNode.createChildSceneNode("tower4");
    Lag::SceneNode &light1Node = tower1Node.createChildSceneNode("light1");
    Lag::SceneNode &light2Node = tower2Node.createChildSceneNode("light2");
    Lag::SceneNode &light3Node = tower3Node.createChildSceneNode("light3");
    Lag::SceneNode &light4Node = tower4Node.createChildSceneNode("light4");

    boardBaseNode.setScale(glm::vec3(BOARD_SIZE, 1.0f, BOARD_SIZE));
    boardBaseNode.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    boardLeftNode.setPosition(glm::vec3(-HALF_BOARD_SIZE, HALF_WALL_HEIGHT, 0.0f));
    boardLeftNode.setScale(glm::vec3(1.0f, WALL_HEIGHT, BOARD_SIZE));

    boardRightNode.setPosition(glm::vec3(HALF_BOARD_SIZE, HALF_WALL_HEIGHT, 0.0f));
    boardRightNode.setScale(glm::vec3(1.0f, WALL_HEIGHT, BOARD_SIZE));

    boardTopNode.setPosition(glm::vec3(0.0f, HALF_WALL_HEIGHT, -HALF_BOARD_SIZE));
    boardTopNode.setScale(glm::vec3(BOARD_SIZE, WALL_HEIGHT, 1.0f));

    boardBottomNode.setPosition(glm::vec3(0.0f, HALF_WALL_HEIGHT, HALF_BOARD_SIZE));
    boardBottomNode.setScale(glm::vec3(BOARD_SIZE, WALL_HEIGHT, 1.0f));

    tower1Node.setPosition(glm::vec3(-HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, -HALF_BOARD_SIZE));
    tower1Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower2Node.setPosition(glm::vec3(HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, HALF_BOARD_SIZE));
    tower2Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower3Node.setPosition(glm::vec3(HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, -HALF_BOARD_SIZE));
    tower3Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));
    tower4Node.setPosition(glm::vec3(-HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, HALF_BOARD_SIZE));
    tower4Node.setScale(glm::vec3(1.0f, TOWER_HEIGHT, 1.0f));

    light1Node.setInheritScale(false);
    light1Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light2Node.setInheritScale(false);
    light2Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light3Node.setInheritScale(false);
    light3Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);
    light4Node.setInheritScale(false);
    light4Node.setPosition(glm::vec3(0.0, HALF_TOWER_HEIGHT, 0.0), Lag::TransformSpace::PARENT);

    const Lag::Color color1(0.08f, 0.06f, 0.9f);
    const Lag::Color color2(0.01f, 0.04f, 0.4f);
    Plane *basePlane = new Plane(color1, color2);
    basePlane->attachToSceneNode(boardBaseNode);
    addEntity(basePlane);

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


    const glm::vec3 TOWER_LIGHT_ATTENUATIONS = glm::vec3(1.0f, 0.3f, 0.3f);

    Lag::PointLight &light1 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light1.attachToSceneNode(light1Node);

    Lag::PointLight &light2 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light2.attachToSceneNode(light2Node);

    Lag::PointLight &light3 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light3.attachToSceneNode(light3Node);

    Lag::PointLight &light4 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light4.attachToSceneNode(light4Node);
}

void MainScene::initPaddles(Lag::SceneNode &parentNode) {
    Lag::SceneNode &paddle1Node = parentNode.createChildSceneNode("paddle1");

    paddle1Node.setPosition(glm::vec3(-HALF_BOARD_SIZE + 1.0f, 0.5, 0.0f), Lag::TransformSpace::WORLD);
    paddle1Node.setScale(glm::vec3(0.5f, 0.5f, 3.0f));

    Paddle *paddle1 = new Paddle();
    addEntity(paddle1);
    paddle1->attachToSceneNode(paddle1Node);
}

void MainScene::initGround(Lag::SceneNode &parentNode) {
    Lag::SceneNode &groundNode = parentNode.createChildSceneNode("groundNode");
    groundNode.setScale(glm::vec3(GROUND_SIZE, 1.0f, GROUND_SIZE));
    groundNode.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));

    const Lag::Color color1(0.1f, 0.2f, 0.25f);
    const Lag::Color color2(0.05f, 0.5f, 0.1f);

    Plane *groundPlane = new Plane(color1, color2);
    groundPlane->attachToSceneNode(groundNode);
    addEntity(groundPlane);
}
