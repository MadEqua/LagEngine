#include "TestApplication.h"

#include "Root.h"
#include "renderer/RenderWindow.h"
#include "scene/SceneManager.h"
#include "io/InputManager.h"
#include "scene/FreeCamera.h"
#include "scene/SceneNode.h"
#include "scene/Entity.h"
#include "io/log/LogManager.h"
#include "scene/PointLight.h"
#include "scene/DirectionalLIght.h"
#include "renderer/Color.h"

#include <iostream>

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
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_IDE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_IDE);

	if (!root->initializeLag("startup.ini"))
		return false;

	renderWindow = &root->getRenderWindow();
	sceneManager = &root->getSceneManager();

	root->registerObserver(*this);
	root->getInputManager().registerObserver(static_cast<Lag::IKeyboardListener&>(*this));
	root->getInputManager().registerObserver(static_cast<Lag::ICursorListener&>(*this));

	camera = new Lag::FreeCamera(45.0f, 0.1f, 1000.0f, 10.0f);
	camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));
	renderWindow->createViewport("default", camera->getCamera());

	/*Lag::Camera &secondaryCamera = sceneManager->createCamera("secondary", 45.0f, 0.1f, 100.0f);
	Lag::SceneNode &secondaryCameraNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("secondaryCamera");
	secondaryCameraNode.setPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	secondaryCameraNode.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), Lag::WORLD);
	secondaryCamera.attachToSceneNode(secondaryCameraNode);
	renderWindow->createViewport("secondary", secondaryCamera, 0.5f, 0.0f, 0.5f, 0.5f);*/

	Lag::PointLight& pl = sceneManager->createPointLight("pl1", Lag::Color(0.0f, 1.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl1SceneNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("pl1SceneNode");
	sceneManager->createEntity("sphere", "sphere", "testMaterial2")->attachToSceneNode(pl1SceneNode);
	pl1SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl1SceneNode.setScale(glm::vec3(0.1f));
	pl.attachToSceneNode(pl1SceneNode);

	Lag::PointLight& pl2 = sceneManager->createPointLight("pl2", Lag::Color(1.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	Lag::SceneNode &pl2SceneNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("pl2SceneNode");
	sceneManager->createEntity("sphere", "sphere", "testMaterial2")->attachToSceneNode(pl2SceneNode);
	pl2SceneNode.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl2SceneNode.setScale(glm::vec3(0.1f));
	pl2.attachToSceneNode(pl2SceneNode);

	Lag::DirectionalLight& dir1 = sceneManager->createDirectionalLight("dir1", Lag::Color(0.3f), glm::vec3(1.0f, -1.0f, 0.0f));
	dir1.attachToSceneNode(sceneManager->getSceneGraph().getRootSceneNode());

	/*Lag::DirectionalLight& dir2 = sceneManager->createDirectionalLight("dir2", Lag::Color(0.3f), glm::vec3(-1.0f, -1.0f, 0.0f));
	dir2.attachToSceneNode(sceneManager->getSceneGraph().getRootSceneNode());*/

	createScene();

	/*Lag::SceneNode &mainNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	mainNode.scale(glm::vec3(0.07f));
	Lag::Entity *ent = sceneManager->createEntity("sponza", "sponza", "testMaterial");
	ent->attachToSceneNode(mainNode);*/

	sceneManager->enableSky("skyMaterial");

	root->startRenderingLoop();

	return true;
}

void TestApplication::createScene()
{
	Lag::SceneNode &mainNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	mainNode.setScale(glm::vec3(0.01f));
	Lag::Entity *ent = sceneManager->createEntity("bunny", "bunny", "testMaterial");
	ent->attachToSceneNode(mainNode);

	Lag::SceneNode &baseNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("base");
	//baseNode.translate(glm::vec3(0.0f, 10.1f, 0.0f), Lag::WORLD);
	baseNode.setScale(glm::vec3(30.0f, 0.3f, 30.0f));
	Lag::Entity *base = sceneManager->createEntity("cube", "cube", "testMaterial2");
	base->attachToSceneNode(baseNode);

	createSceneAux(mainNode, 10.0f, 8, 0, 3);
}

void TestApplication::createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth)
{
	if (actualdepth >= maxdepth) return;

	for (int i = 0; i < count; ++i)
	{
		Lag::SceneNode &periferyNode = center.createChildSceneNode("perifery" + 
			std::to_string(actualdepth) + std::to_string(i));

		periferyNode.scale(glm::vec3(0.6f));

		periferyNode.yaw(static_cast<float>(i) * (360.0f / static_cast<float>(count)), Lag::PARENT);
		periferyNode.translate(glm::vec3(size, 0.0f, 0.0f), Lag::LOCAL);

		Lag::Entity *periferyEnt = sceneManager->createEntity("perifery" + 
			std::to_string(actualdepth) + std::to_string(i), "bunny", "testMaterial");

		periferyEnt->attachToSceneNode(periferyNode);

		createSceneAux(periferyNode, size * 0.3f, count, actualdepth + 1, maxdepth);
	}
}

void TestApplication::updateSceneAux(int count, int actualdepth, int maxdepth)
{
	if (actualdepth >= maxdepth) return;

	for (int i = 0; i < count; ++i)
	{
		Lag::SceneNode *periferyNode = sceneManager->getSceneGraph().
			getSceneNode("perifery" + std::to_string(actualdepth) + std::to_string(i));

		//std::cout << "perifery" + std::to_string(actualdepth) + std::to_string(i) << std::endl;

		float s = glm::sin(time) / 100.0f;
		periferyNode->translate(glm::vec3(s, 0, 0.0f), Lag::LOCAL);

		updateSceneAux(count, actualdepth + 1, maxdepth);
	}
}

void TestApplication::onFrameStart(float timePassed)
{
}

void TestApplication::onFrameRenderingQueued(float timePassed)
{
	Lag::SceneNode *pl1SceneNode = sceneManager->getSceneGraph().getSceneNode("pl1SceneNode");
	float cycle = glm::sin(time * glm::pi<float>() / 10.0f);
	glm::vec3 posCpy = pl1SceneNode->getWorldPosition();
	posCpy.x = cycle * 15.0f;
	pl1SceneNode->setPosition(posCpy);

	Lag::SceneNode *pl2SceneNode = sceneManager->getSceneGraph().getSceneNode("pl2SceneNode");
	cycle = glm::sin(time * glm::pi<float>() / 11.0f);
	posCpy = pl2SceneNode->getWorldPosition();
	posCpy.z = cycle * 11.0f;
	pl2SceneNode->setPosition(posCpy);

	Lag::SceneNode *mainNode = sceneManager->getSceneGraph().getSceneNode("main");
	cycle = glm::sin(time * glm::pi<float>() / 15.0f);
	posCpy = mainNode->getWorldPosition();
	posCpy.y = cycle * 1.0f;
	mainNode->setPosition(posCpy);
	mainNode->yaw(timePassed * 30.0f, Lag::LOCAL);
	
	//updateSceneAux(8, 0, 3);

	time += timePassed;
}

void TestApplication::onFrameEnd(float timePassed)
{
}

void TestApplication::onKeyPress(int key, int modifier)
{

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