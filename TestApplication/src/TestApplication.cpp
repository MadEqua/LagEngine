#include "TestApplication.h"

#include "Root.h"
#include "renderer/RenderWindow.h"
#include "scene/SceneManager.h"
#include "io/InputManager.h"
#include "scene/FreeCamera.h"
#include "scene/SceneNode.h"
#include "scene/Entity.h"
#include "io/log/LogManager.h"

TestApplication::TestApplication()
{
}

TestApplication::~TestApplication()
{
	delete camera;
}

bool TestApplication::start()
{
	root = &Lag::Root::getInstance();

	Lag::LogManager::getInstance().removeFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_FILE);
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

	camera = new Lag::FreeCamera(45.0f, 0.1f, 100.0f, 0.3f);
	camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 0, 20));
	renderWindow->createViewport("default", camera->getCamera());

	Lag::Camera &secondaryCamera = sceneManager->createCamera("secondary", 45.0f, 0.1f, 100.0f);
	Lag::SceneNode &secondaryCameraNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("secondaryCamera");
	secondaryCameraNode.setPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	secondaryCameraNode.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), Lag::WORLD);
	secondaryCamera.attachToSceneNode(secondaryCameraNode);
	renderWindow->createViewport("secondary", secondaryCamera, 0.5f, 0.0f, 0.5f, 0.5f);

	createScene();

	root->startRenderingLoop();

	return true;
}

void TestApplication::createScene()
{
	Lag::SceneNode &mainNode = sceneManager->getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	Lag::Entity *ent = sceneManager->createEntity("sphere", "sphere", "testMaterial");
	ent->attachToSceneNode(mainNode);

	createSceneAux(mainNode, 20.0f, 8, 0, 3);
}

void TestApplication::createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth)
{
	if (actualdepth >= maxdepth) return;

	for (int i = 0; i < count; ++i)
	{
		Lag::SceneNode &periferyNode = center.createChildSceneNode("perifery" + actualdepth + i);
		periferyNode.scale(center.getLocalScale() * 0.7f);

		periferyNode.yaw(static_cast<float>(i) * (360.0f / static_cast<float>(count)), Lag::PARENT);
		periferyNode.translate(glm::vec3(size, 0.0f, 0.0f), Lag::LOCAL);

		Lag::Entity *periferyEnt = sceneManager->createEntity("perifery" + actualdepth + i, "sphere", "testMaterial");
		periferyEnt->attachToSceneNode(periferyNode);

		createSceneAux(periferyNode, size * 0.3f, count, actualdepth + 1, maxdepth);
	}
}

void TestApplication::onFrameStart(float timePassed)
{
}

void TestApplication::onFrameRenderingQueued(float timePassed)
{
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