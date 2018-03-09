#include "TestScene.h"

#include "Root.h"
#include "renderer/RenderTargetManager.h"
#include "renderer/RenderWindow.h"
#include "utils/FreeCamera.h"
#include "scene/OrthographicCamera.h"
#include "scene/SceneNode.h"
#include "scene/Entity.h"
#include "scene/PointLight.h"
#include "scene/DirectionalLIght.h"
#include "renderer/Color.h"
#include "io/Keys.h"

TestScene::TestScene(bool isScene1) :
	isScene1(isScene1),
	time(0.0f),
	nodeName(0)
{
}

void TestScene::onStart()
{
	Lag::Root &root = Lag::Root::getInstance();
	
	/*Lag::Camera &secondaryCamera = sceneManager->createCamera("secondary", 45.0f, 0.1f, 100.0f);
	Lag::SceneNode &secondaryCameraNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("secondaryCamera");
	secondaryCameraNode.setPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	secondaryCameraNode.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), Lag::WORLD);
	secondaryCamera.attachToSceneNode(secondaryCameraNode);
	renderWindow->createViewport("secondary", secondaryCamera, 0.5f, 0.0f, 0.5f, 0.5f);*/

	Lag::PointLight& pl = createPointLight(isScene1 ? Lag::Color(0.0f, 1.0f, 0.0f) : Lag::Color(0.0f, 0.0f, 1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl1SceneNode = sceneGraph.getRootSceneNode().createChildSceneNode("pl1SceneNode");
	createEntity("cube", "baseMaterial")->attachToSceneNode(pl1SceneNode);
	pl1SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl1SceneNode.setScale(glm::vec3(0.1f));
	pl.attachToSceneNode(pl1SceneNode);

	Lag::PointLight& pl2 = createPointLight(isScene1 ? Lag::Color(1.0f, 0.0f, 0.0f) : Lag::Color(0.5f, 0.5f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl2SceneNode = sceneGraph.getRootSceneNode().createChildSceneNode("pl2SceneNode");
	createEntity("cube", "baseMaterial")->attachToSceneNode(pl2SceneNode);
	pl2SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl2SceneNode.setScale(glm::vec3(0.1f));
	pl2.attachToSceneNode(pl2SceneNode);

	Lag::DirectionalLight& dir1 = createDirectionalLight(Lag::Color(0.3f), glm::vec3(1.0f, -1.0f, 0.0f));

	Lag::SceneNode &mainNode = getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	mainNode.setScale(glm::vec3(0.9f));
	Lag::Entity *ent = createEntity("piano", "objectMaterial");
	ent->attachToSceneNode(mainNode);

	Lag::SceneNode &floorNode = getSceneGraph().getRootSceneNode().createChildSceneNode("floor");
	floorNode.translate(glm::vec3(0.0f, -0.3f, 0.0f), Lag::WORLD);
	floorNode.setScale(glm::vec3(30.0f, 0.3f, 30.0f));
	Lag::Entity *floor = createEntity("cube", "baseMaterial");
	floor->attachToSceneNode(floorNode);

	createSceneAux(mainNode, 10.0f, 8, 0, 3);

	enableSky("skyMaterial");
}

void TestScene::onEnd()
{
	delete camera;
}

void TestScene::onInitializeViewports(Lag::RenderWindow &renderWindow)
{
	camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
	camera->getCamera().getParentSceneNode()->setPosition(isScene1 ? glm::vec3(0, 10, 20) : glm::vec3(0, 20, 30));

	renderWindow.createViewport(camera->getCamera());
}

void TestScene::createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth)
{
	if (actualdepth >= maxdepth) return;

	for (int i = 0; i < count; ++i)
	{
		Lag::SceneNode &periferyNode = center.createChildSceneNode("perifery" + 
			std::to_string(actualdepth) +
			std::to_string(maxdepth) + "-" +
			std::to_string(nodeName++));

		periferyNode.scale(glm::vec3(0.5f));

		periferyNode.yaw(static_cast<float>(i) * (360.0f / static_cast<float>(count)), Lag::PARENT);
		periferyNode.translate(glm::vec3(size, 0.0f, 0.0f), Lag::LOCAL);

		Lag::Entity *periferyEnt = createEntity("piano", "objectMaterial");

		periferyEnt->attachToSceneNode(periferyNode);

		createSceneAux(periferyNode, size * 0.3f, count, actualdepth + 1, maxdepth);
	}
}

void TestScene::updateScene(float timePassed)
{
	Lag::SceneNode *pl1SceneNode = sceneGraph.getSceneNode("pl1SceneNode");
	float cycle = glm::sin(time * glm::pi<float>() / 10.0f);
	glm::vec3 posCpy = pl1SceneNode->getWorldPosition();
	posCpy.x = cycle * 15.0f;
	pl1SceneNode->setPosition(posCpy);

	Lag::SceneNode *pl2SceneNode = sceneGraph.getSceneNode("pl2SceneNode");
	cycle = glm::sin(time * glm::pi<float>() / 11.0f);
	posCpy = pl2SceneNode->getWorldPosition();
	posCpy.z = cycle * 11.0f;
	pl2SceneNode->setPosition(posCpy);

	/*Lag::SceneNode *mainNode = scene->getSceneGraph().getSceneNode("main");
	cycle = glm::sin(time * glm::pi<float>() / 15.0f);
	posCpy = mainNode->getWorldPosition();
	posCpy.y = cycle * 0.2f;
	mainNode->setPosition(posCpy);
	mainNode->yaw(timePassed * 30.0f, Lag::LOCAL);*/
}

void TestScene::onFrameRenderingQueued(float timePassed)
{
	updateScene(timePassed);
	time += timePassed;
}

void TestScene::onKeyPress(int key, int modifier)
{
	if(key == KEY_1)
		scheduleSceneChange(isScene1 ? "scene2" : "scene1");
}