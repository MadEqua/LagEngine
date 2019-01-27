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
    initBallsAndPaddles(rootNode);
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

void MainScene::initBallsAndPaddles(Lag::SceneNode &parentNode) {
    const int BALL_COUNT = 4;
    
    std::vector<Ball*> balls(BALL_COUNT);

    for(int i = 0; i < BALL_COUNT; ++i) {
        Ball *ball = new Ball(*this, parentNode, "ball" + i);
        balls[i] = ball;
    }

    const float PADDLE_SMALL_SIZE = 0.6f;
    const float PADDLE_LARGE_SIZE = 4.0f;

    Paddle *paddle1 = new Paddle(*this, parentNode, "paddle1", balls, glm::vec3(-HALF_BOARD_SIZE + 2.0f, 0.75, 0.0f),
                                 glm::vec3(PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE, PADDLE_LARGE_SIZE),
                                 glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    Paddle *paddle2 = new Paddle(*this, parentNode, "paddle2", balls, glm::vec3(HALF_BOARD_SIZE - 2.0f, 0.75, 0.0f),
                                 glm::vec3(PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE, PADDLE_LARGE_SIZE),
                                 glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    Paddle *paddle3 = new Paddle(*this, parentNode, "paddle3", balls, glm::vec3(0.0f, 0.75, -HALF_BOARD_SIZE + 2.0f),
                                 glm::vec3(PADDLE_LARGE_SIZE, PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE),
                                 glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    Paddle *paddle4 = new Paddle(*this, parentNode, "paddle4", balls, glm::vec3(0.0f, 0.75, HALF_BOARD_SIZE - 2.0f),
                                 glm::vec3(PADDLE_LARGE_SIZE, PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE),
                                 glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

void MainScene::initBoard(Lag::SceneNode &parentNode) {
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

    const Lag::Color COLOR_1(0.08f, 0.06f, 0.9f);
    const Lag::Color COLOR_2(0.01f, 0.04f, 0.4f);
    Plane *basePlane = new Plane(*this, parentNode, "boardBase",
                                    glm::vec3(0.0f),
                                    glm::vec3(BOARD_SIZE, 1.0f, BOARD_SIZE),
                                    COLOR_1, COLOR_2, 1.3f, 6.0f, 0.03f);

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


    const glm::vec3 TOWER_LIGHT_ATTENUATIONS = glm::vec3(1.0f, 0.35f, 0.35f);

    Lag::PointLight &light1 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light1.attachToSceneNode(light1Node);

    Lag::PointLight &light2 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light2.attachToSceneNode(light2Node);

    Lag::PointLight &light3 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light3.attachToSceneNode(light3Node);

    Lag::PointLight &light4 = createPointLight(Lag::Color(2.0f), TOWER_LIGHT_ATTENUATIONS);
    light4.attachToSceneNode(light4Node);
}

void MainScene::initGround(Lag::SceneNode &parentNode) {
    const float SMALL_BIAS = 1.0f;

    const Lag::Color COLOR_1(0.1f, 0.2f, 0.25f);
    const Lag::Color COLOR_2(0.05f, 0.5f, 0.1f);

    const float TRIS_PER_LENGTH = 2.0f;
    const float MAX_POINT_SIZE = 3.0f;
    const float DISPLACEMENT_STRENGTH = 0.002f;

    Plane *groundPlane1 = new Plane(*this, parentNode, "groundNode1", 
                                    glm::vec3(-HALF_BOARD_SIZE - HALF_GROUND_SIZE - SMALL_BIAS, 0.0f, 0.0f), 
                                    glm::vec3(GROUND_SIZE, 1.0f, GROUND_SIZE),
                                    COLOR_1, COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE, DISPLACEMENT_STRENGTH);

    Plane *groundPlane2 = new Plane(*this, parentNode, "groundNode2",
                                    glm::vec3(HALF_BOARD_SIZE + HALF_GROUND_SIZE + SMALL_BIAS, 0.0f, 0.0f),
                                    glm::vec3(GROUND_SIZE, 1.0f, GROUND_SIZE),
                                    COLOR_1, COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE, DISPLACEMENT_STRENGTH);

    Plane *groundPlane3 = new Plane(*this, parentNode, "groundNode3",
                                    glm::vec3(0.0f, 0.0f, HALF_BOARD_SIZE + HALF_GROUND_SIZE + SMALL_BIAS),
                                    glm::vec3(GROUND_SIZE, 1.0f, GROUND_SIZE),
                                    COLOR_1, COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE, DISPLACEMENT_STRENGTH);

    Plane *groundPlane4 = new Plane(*this, parentNode, "groundNode4",
                                    glm::vec3(0.0f, 0.0f, -HALF_BOARD_SIZE - HALF_GROUND_SIZE - SMALL_BIAS),
                                    glm::vec3(GROUND_SIZE, 1.0f, GROUND_SIZE),
                                    COLOR_1, COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE, DISPLACEMENT_STRENGTH);
}