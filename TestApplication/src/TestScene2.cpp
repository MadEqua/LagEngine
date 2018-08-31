#include "TestScene2.h"

#include "Root.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "PerspectiveCamera.h"
#include "FreeCamera.h"
#include "Entity.h"

void TestScene2::onStart() {
    Lag::Root &root = Lag::Root::getInstance();

    root.getRenderer().setPointSizeFromGpuProgramEnabled(true);
    root.getRenderer().setClearColor(Lag::Color(0.1f));

    Lag::SceneNode &rootNode = getSceneGraph().getRootSceneNode();

    Lag::SceneNode &boardNode = rootNode.createChildSceneNode("board");
    Lag::SceneNode &boardBaseNode = boardNode.createChildSceneNode("boardBase");
    Lag::SceneNode &boardLeftNode = rootNode.createChildSceneNode("boardLeft");
    Lag::SceneNode &boardRightNode = boardNode.createChildSceneNode("boardRight");
    Lag::SceneNode &boardTopNode = boardNode.createChildSceneNode("boardTop");
    Lag::SceneNode &boardBottomNode = boardNode.createChildSceneNode("boardBottom");


    boardBaseNode.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    boardBaseNode.setScale(glm::vec3(10.0f, 10.0f, 10.0f));

    Lag::Entity *baseCube = createEntity("cube", "pointMaterial");
    baseCube->attachToSceneNode(boardBaseNode);


    /*Lag::SceneNode &bunnyNode = mainNode.createChildSceneNode("bunnyNode");
    bunnyNode.setScale(glm::vec3(20.0f));
    Lag::Entity *bunny = createEntity("bunny", "pointMaterial");
    bunny->attachToSceneNode(bunnyNode);

    Lag::SceneNode &treeNode = mainNode.createChildSceneNode("treeNode");
    treeNode.setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    Lag::Entity *tree = createEntity("tree1", "pointMaterial");
    tree->attachToSceneNode(treeNode);*/

    //Lag::Entity *sponza = createEntity("sponza", "baseMaterial");
    //sponza->attachToSceneNode(mainNode);
}

void TestScene2::onEnd() {
    delete camera;
}

void TestScene2::onInitializeViewports(Lag::RenderWindow &renderWindow) {
    camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
    camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));

    renderWindow.createViewport(camera->getCamera());
}