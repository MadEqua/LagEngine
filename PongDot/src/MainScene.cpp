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
#include "Box.h"
#include "Star.h"


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
    camera->getCamera().getParentSceneNode()->lookAt(glm::vec3(20, 30, 20), glm::vec3(0), glm::vec3(0, 1, 0));
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
        Ball *ball = new Ball(*this, parentNode, "ball" + std::to_string(i));
        balls[i] = ball;
    }

    const float PADDLE_SMALL_SIZE = 0.6f;
    const float PADDLE_LARGE_SIZE = 4.0f;
    const float MARGIN_TO_BOARD = 1.5f;

    Paddle *paddle1 = new Paddle(*this, parentNode, "paddle1", balls, glm::vec3(-HALF_BOARD_SIZE + MARGIN_TO_BOARD, 0.75, 0.0f),
                                 glm::vec3(PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE, PADDLE_LARGE_SIZE),
                                 glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    Paddle *paddle2 = new Paddle(*this, parentNode, "paddle2", balls, glm::vec3(HALF_BOARD_SIZE - MARGIN_TO_BOARD, 0.75, 0.0f),
                                 glm::vec3(PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE, PADDLE_LARGE_SIZE),
                                 glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    Paddle *paddle3 = new Paddle(*this, parentNode, "paddle3", balls, glm::vec3(0.0f, 0.75, -HALF_BOARD_SIZE + MARGIN_TO_BOARD),
                                 glm::vec3(PADDLE_LARGE_SIZE, PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE),
                                 glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    Paddle *paddle4 = new Paddle(*this, parentNode, "paddle4", balls, glm::vec3(0.0f, 0.75, HALF_BOARD_SIZE - MARGIN_TO_BOARD),
                                 glm::vec3(PADDLE_LARGE_SIZE, PADDLE_SMALL_SIZE, PADDLE_SMALL_SIZE),
                                 glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

void MainScene::initBoard(Lag::SceneNode &parentNode) {

    const float WALL_HEIGHT = 2.0f;
    const float HALF_WALL_HEIGHT = WALL_HEIGHT * 0.5f;

    const float TOWER_HEIGHT = WALL_HEIGHT * 6.0f;
    const float HALF_TOWER_HEIGHT = TOWER_HEIGHT * 0.5f;

    const Lag::Color BOX_COLOR_1(0.08f, 0.05f, 0.1f);
    const Lag::Color BOX_COLOR_2(0.02f, 0.1f, 0.85f);

    const float TRIS_PER_LENGTH = 5.0f;
    const glm::vec2 MAX_POINT_SIZE_LIMITS(6.0f, 10.0f);
    const glm::vec2 DISPLACEMENT_STRENGTH_LIMITS(0.04f, 0.005f);

    Box *boardLeft = new Box(*this, parentNode, "boardLeft",
                             glm::vec3(-HALF_BOARD_SIZE, HALF_WALL_HEIGHT, 0.0f),
                             glm::vec3(1.0f, WALL_HEIGHT, BOARD_SIZE),
                             BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *boardRight = new Box(*this, parentNode, "boardRight",
                              glm::vec3(HALF_BOARD_SIZE, HALF_WALL_HEIGHT, 0.0f),
                              glm::vec3(1.0f, WALL_HEIGHT, BOARD_SIZE),
                              BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *boardTop = new Box(*this, parentNode, "boardTop",
                            glm::vec3(0.0f, HALF_WALL_HEIGHT, -HALF_BOARD_SIZE),
                            glm::vec3(BOARD_SIZE, WALL_HEIGHT, 1.0f),
                            BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *boardBottom = new Box(*this, parentNode, "boardBottom",
                               glm::vec3(0.0f, HALF_WALL_HEIGHT, HALF_BOARD_SIZE),
                               glm::vec3(BOARD_SIZE, WALL_HEIGHT, 1.0f),
                               BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);


    Box *tower1 = new Box(*this, parentNode, "tower1",
                          glm::vec3(-HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, -HALF_BOARD_SIZE),
                          glm::vec3(1.0f, TOWER_HEIGHT, 1.0f),
                          BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *tower2 = new Box(*this, parentNode, "tower2",
                          glm::vec3(HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, HALF_BOARD_SIZE),
                          glm::vec3(1.0f, TOWER_HEIGHT, 1.0f),
                          BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *tower3 = new Box(*this, parentNode, "tower3",
                          glm::vec3(HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, -HALF_BOARD_SIZE),
                          glm::vec3(1.0f, TOWER_HEIGHT, 1.0f),
                          BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);

    Box *tower4 = new Box(*this, parentNode, "tower4",
                          glm::vec3(-HALF_BOARD_SIZE, HALF_TOWER_HEIGHT, HALF_BOARD_SIZE),
                          glm::vec3(1.0f, TOWER_HEIGHT, 1.0f),
                          BOX_COLOR_1, BOX_COLOR_2, TRIS_PER_LENGTH, MAX_POINT_SIZE_LIMITS, DISPLACEMENT_STRENGTH_LIMITS);


    const Lag::Color BOARD_COLOR_1(0.08f, 0.06f, 0.9f);
    const Lag::Color BOARD_COLOR_2(0.01f, 0.04f, 0.4f);
    Plane *basePlane = new Plane(*this, parentNode, "boardBase",
                                    glm::vec3(0.0f),
                                    glm::vec3(BOARD_SIZE, 1.0f, BOARD_SIZE),
                                    BOARD_COLOR_1, BOARD_COLOR_2, 1.0f, 6.0f, 0.02f);
}

void MainScene::initGround(Lag::SceneNode &parentNode) {
    const float GROUND_SIZE = BOARD_SIZE * 6.0f;
    const float HALF_GROUND_SIZE = GROUND_SIZE * 0.5f;

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


    
    for(int i = 0; i < 50; ++i) {
        glm::vec3 position(glm::linearRand(-GROUND_SIZE * 0.5f, GROUND_SIZE * 0.5f) * 2.0f,
                           glm::linearRand(50.0f, 250.0f), 
                           glm::linearRand(-GROUND_SIZE * 0.5f, GROUND_SIZE * 0.5f) * 2.0f);
        glm::vec3 scale(glm::linearRand(0.5f, 1.0f));
        Star *star = new Star(*this, parentNode, "star" + std::to_string(i), position, scale, *camera->getCamera().getParentSceneNode());
    }
}