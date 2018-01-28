#include "TestApplication.h"

#include <iostream>

#include "Root.h"
#include "renderer/RenderWindow.h"
#include "scene/SceneManager.h"
#include "scene/Scene.h"
#include "io/InputManager.h"
#include "scene/FreeCamera.h"
#include "scene/OrthographicCamera.h"
#include "scene/SceneNode.h"
#include "scene/Entity.h"
#include "io/log/LogManager.h"
#include "scene/PointLight.h"
#include "scene/DirectionalLIght.h"
#include "renderer/Color.h"
#include "io/Keys.h"


TestApplication::TestApplication() :
	time(0.0f)
{
}

TestApplication::~TestApplication()
{
	delete camera;
}

bool TestApplication::start()
{
	root = &Lag::Root::getInstance();

	//Lag::LogManager::getInstance().removeFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_FILE);
	
	//Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_INFO, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_INFO, Lag::LAG_LOG_OUT_IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_WARNING, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_WARNING, Lag::LAG_LOG_OUT_IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_IDE);

	//Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_IDE);

	if (!root->initializeLag("startup.ini"))
		return false;

	renderWindow = &root->getRenderWindow();

	scene1 = &root->getSceneManager().createScene("scene1");
	root->getSceneManager().setCurrentScene("scene1");

	//scene2 = &root->getSceneManager().createScene("scene2");

	root->getRenderer().registerObserver(*this);
	root->getInputManager().registerObserver(static_cast<Lag::IKeyboardListener&>(*this));
	root->getInputManager().registerObserver(static_cast<Lag::ICursorListener&>(*this));

	createScene(scene1, true);
	//createScene(scene2, false);

	root->startRenderingLoop();

	return true;
}

void TestApplication::createScene(Lag::Scene *scene, bool isOne)
{
	camera = new Lag::FreeCamera(45.0f, 0.1f, 1000.0f, 10.0f);
	camera->getCamera().getParentSceneNode()->setPosition(isOne ? glm::vec3(0, 10, 20) : glm::vec3(0, 20, 30));
	renderWindow->createViewport(camera->getCamera());

	/*Lag::Camera &secondaryCamera = sceneManager->createCamera("secondary", 45.0f, 0.1f, 100.0f);
	Lag::SceneNode &secondaryCameraNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("secondaryCamera");
	secondaryCameraNode.setPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	secondaryCameraNode.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), Lag::WORLD);
	secondaryCamera.attachToSceneNode(secondaryCameraNode);
	renderWindow->createViewport("secondary", secondaryCamera, 0.5f, 0.0f, 0.5f, 0.5f);*/

	Lag::PointLight& pl = scene->createPointLight(isOne ? Lag::Color(0.0f, 1.0f, 0.0f) : Lag::Color(0.0f, 0.0f, 1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl1SceneNode = scene->getSceneGraph().getRootSceneNode().createChildSceneNode("pl1SceneNode");
	scene->createEntity("sphere", "testMaterial2")->attachToSceneNode(pl1SceneNode);
	pl1SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl1SceneNode.setScale(glm::vec3(0.1f));
	pl.attachToSceneNode(pl1SceneNode);

	Lag::PointLight& pl2 = scene->createPointLight(isOne ? Lag::Color(1.0f, 0.0f, 0.0f) : Lag::Color(0.5f, 0.5f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl2SceneNode = scene->getSceneGraph().getRootSceneNode().createChildSceneNode("pl2SceneNode");
	scene->createEntity("sphere", "testMaterial2")->attachToSceneNode(pl2SceneNode);
	pl2SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl2SceneNode.setScale(glm::vec3(0.1f));
	pl2.attachToSceneNode(pl2SceneNode);

	Lag::DirectionalLight& dir1 = scene->createDirectionalLight(Lag::Color(0.3f), glm::vec3(1.0f, -1.0f, 0.0f));

	Lag::SceneNode &mainNode = scene->getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	mainNode.setScale(glm::vec3(0.01f));
	Lag::Entity *ent = scene->createEntity("bunny", "testMaterial");
	ent->attachToSceneNode(mainNode);

	Lag::SceneNode &baseNode = scene->getSceneGraph().getRootSceneNode().createChildSceneNode("base");
	baseNode.translate(glm::vec3(0.0f, -0.9f, 0.0f), Lag::WORLD);
	baseNode.setScale(glm::vec3(30.0f, 0.3f, 30.0f));
	Lag::Entity *base = scene->createEntity("cube", "testMaterial2");
	base->attachToSceneNode(baseNode);

	createSceneAux(scene, mainNode, 10.0f, 8, 0, 3);

	/*Lag::SceneNode &mainNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	mainNode.scale(glm::vec3(0.07f));
	Lag::Entity *ent = sceneManager->createEntity("sponza", "sponza", "testMaterial");
	ent->attachToSceneNode(mainNode);*/

	scene->enableSky("skyMaterial");
}

void TestApplication::createSceneAux(Lag::Scene *scene, Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth)
{
	if (actualdepth >= maxdepth) return;

	for (int i = 0; i < count; ++i)
	{
		Lag::SceneNode &periferyNode = center.createChildSceneNode("perifery" + std::to_string(actualdepth) + std::to_string(i));

		periferyNode.scale(glm::vec3(0.6f));

		periferyNode.yaw(static_cast<float>(i) * (360.0f / static_cast<float>(count)), Lag::PARENT);
		periferyNode.translate(glm::vec3(size, 0.0f, 0.0f), Lag::LOCAL);

		Lag::Entity *periferyEnt = scene->createEntity("bunny", "testMaterial");

		periferyEnt->attachToSceneNode(periferyNode);

		createSceneAux(scene, periferyNode, size * 0.3f, count, actualdepth + 1, maxdepth);
	}
}

void TestApplication::updateScene(float timePassed)
{
	Lag::Scene *scene = &root->getSceneManager().getCurrentScene();
	
	Lag::SceneNode *pl1SceneNode = scene->getSceneGraph().getSceneNode("pl1SceneNode");
	float cycle = glm::sin(time * glm::pi<float>() / 10.0f);
	glm::vec3 posCpy = pl1SceneNode->getWorldPosition();
	posCpy.x = cycle * 15.0f;
	pl1SceneNode->setPosition(posCpy);

	Lag::SceneNode *pl2SceneNode = scene->getSceneGraph().getSceneNode("pl2SceneNode");
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

void TestApplication::onFrameStart(float timePassed)
{
}

void TestApplication::onFrameRenderingQueued(float timePassed)
{
	updateScene(timePassed);
	time += timePassed;
}

void TestApplication::onFrameEnd(float timePassed)
{
	if (changeScene)
	{
		root->getSceneManager().setCurrentScene(sceneToChange);
		changeScene = false;
	}
}

void TestApplication::onKeyPress(int key, int modifier)
{
	if (key == KEY_1)
	{
		sceneToChange = "scene1";
		changeScene = true;
	}
	else if (key == KEY_2)
	{
		sceneToChange = "scene2";
		changeScene = true;
	}
}

void TestApplication::onKeyRelease(int key, int modifier)
{
}

void TestApplication::onKeyRepeat(int key, int modifier)
{
}

void TestApplication::onCursorMove(int x, int y)
{
}

void TestApplication::onButtonPressed(int x, int y, int button, int modifiers)
{
}

void TestApplication::onButtonReleased(int x, int y, int button, int modifiers)
{
}