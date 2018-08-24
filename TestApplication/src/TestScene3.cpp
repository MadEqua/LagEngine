#include "TestScene3.h"

#include "Root.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "PerspectiveCamera.h"
#include "FreeCamera.h"
#include "Entity.h"


void TestScene3::onStart() {
    Lag::Root &root = Lag::Root::getInstance();

    root.getRenderer().setClearColor(Lag::Color(0.1f));

    Lag::SceneNode &rootNode = getSceneGraph().getRootSceneNode();

    Lag::DirectionalLight &dir1 = createDirectionalLight(Lag::Color(0.5f), glm::vec3(1.0f, -1.0f, 0.0f));

    Lag::SceneNode &floorNode = getSceneGraph().getRootSceneNode().createChildSceneNode("floor");
    floorNode.translate(glm::vec3(0.0f, -0.3f, 0.0f), Lag::TransformSpace::WORLD);
    floorNode.setScale(glm::vec3(30.0f, 0.3f, 30.0f));
    Lag::Entity *floor = createEntity("cube", "baseMaterial");
    floor->attachToSceneNode(floorNode);

    //To test Local translations
    Lag::SceneNode &objectNode = getSceneGraph().getRootSceneNode().createChildSceneNode("localTranslation");
    objectNode.translate(glm::vec3(-10.0f, 0.0f, -10.0f), Lag::TransformSpace::WORLD);
    objectNode.yaw(45.0f, Lag::TransformSpace::WORLD);
    Lag::Entity *object = createEntity("piano", "pianoMaterial");
    object->attachToSceneNode(objectNode);

    //To test Parent translations
    Lag::SceneNode &dummyNode2 = getSceneGraph().getRootSceneNode().createChildSceneNode("dummyNode2");
    dummyNode2.translate(glm::vec3(0.0f, 4.0f, -10.0f), Lag::TransformSpace::WORLD);
    dummyNode2.roll(-90.0f, Lag::TransformSpace::WORLD);
    Lag::SceneNode &objectNode2 = dummyNode2.createChildSceneNode("parentTranslation");
    objectNode2.translate(glm::vec3(0.0f, -2.0f, 0.0f), Lag::TransformSpace::WORLD);
    objectNode2.roll(90.0f, Lag::TransformSpace::WORLD);
    Lag::Entity *object2 = createEntity("piano", "pianoMaterial");
    object2->attachToSceneNode(objectNode2);

    //To test World translations
    Lag::SceneNode &objectNode3 = getSceneGraph().getRootSceneNode().createChildSceneNode("worldTranslation");
    objectNode3.translate(glm::vec3(10.0f, 0.0f, -10.0f), Lag::TransformSpace::WORLD);
    objectNode3.yaw(45.0f, Lag::TransformSpace::WORLD);
    Lag::Entity *object3 = createEntity("piano", "pianoMaterial");
    object3->attachToSceneNode(objectNode3);


    //To test Local rotations
    Lag::SceneNode &objectNode4 = getSceneGraph().getRootSceneNode().createChildSceneNode("localRotation");
    objectNode4.pitch(-45.0f, Lag::TransformSpace::WORLD);
    objectNode4.translate(glm::vec3(-10.0f, 0.0f, 10.0f), Lag::TransformSpace::WORLD);
    Lag::Entity *object4 = createEntity("piano", "pianoMaterial");
    object4->attachToSceneNode(objectNode4);

    //To test Parent rotations
    Lag::SceneNode &dummyNode5 = getSceneGraph().getRootSceneNode().createChildSceneNode("dummyNode5");
    dummyNode5.translate(glm::vec3(0.0f, 2.0f, 10.0f), Lag::TransformSpace::WORLD);
    dummyNode5.pitch(-90.0f, Lag::TransformSpace::WORLD);
    Lag::SceneNode &objectNode5 = dummyNode5.createChildSceneNode("parentRotation");
    objectNode5.translate(glm::vec3(0.0f, -2.0f, 0.0f), Lag::TransformSpace::PARENT);
    objectNode5.pitch(90.0f, Lag::TransformSpace::PARENT);
    Lag::Entity *object5 = createEntity("piano", "pianoMaterial");
    object5->attachToSceneNode(objectNode5);

    //To test World rotations
    Lag::SceneNode &objectNode6 = getSceneGraph().getRootSceneNode().createChildSceneNode("worldRotation");
    objectNode6.translate(glm::vec3(10.0f, 0.0f, 10.0f), Lag::TransformSpace::WORLD);
    objectNode6.pitch(45.0f, Lag::TransformSpace::WORLD);
    Lag::Entity *object6 = createEntity("piano", "pianoMaterial");
    object6->attachToSceneNode(objectNode6);
}

void TestScene3::onEnd() {
    delete camera;
}

void TestScene3::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));

    renderWindow.createViewport(camera->getCamera());
}

void TestScene3::onFrameStart(float timePassed) {

    static float timeTotal = 0.0f;
    timeTotal += timePassed;

    auto localTranslationNode = getSceneGraph().getSceneNode("localTranslation");
    auto parentTranslationNode = getSceneGraph().getSceneNode("parentTranslation");
    auto worldTranslationNode = getSceneGraph().getSceneNode("worldTranslation");

    const float TRANSLATION_UNITS_PER_SECOND = 2.0f;
    float animStep = TRANSLATION_UNITS_PER_SECOND * timePassed;
    float animation = glm::sign(glm::cos(timeTotal * (1.0f / TRANSLATION_UNITS_PER_SECOND) * 3.1415f)) * animStep;

    localTranslationNode->translate(glm::vec3(animation, 0.0f, 0.0f), Lag::TransformSpace::LOCAL);
    parentTranslationNode->translate(glm::vec3(animation, 0.0f, 0.0f), Lag::TransformSpace::PARENT);
    worldTranslationNode->translate(glm::vec3(animation, 0.0f, 0.0f), Lag::TransformSpace::WORLD);


    auto localRotationNode = getSceneGraph().getSceneNode("localRotation");
    auto parentRotationNode = getSceneGraph().getSceneNode("parentRotation");
    auto worldRotationNode = getSceneGraph().getSceneNode("worldRotation");

    const float ROTATION_UNITS_PER_SECOND = 15.0f;
    animation = ROTATION_UNITS_PER_SECOND * timePassed;

    localRotationNode->yaw(animation, Lag::TransformSpace::LOCAL);
    parentRotationNode->yaw(animation, Lag::TransformSpace::PARENT);
    worldRotationNode->yaw(animation, Lag::TransformSpace::WORLD);
}
