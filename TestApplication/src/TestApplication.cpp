#include "TestApplication.h"

#include "Root.h"
#include "LogManager.h"
#include "SceneManager.h"

#include "TestScene.h"
#include "TestScene2.h"

bool TestApplication::start()
{
	Lag::Root &root = Lag::Root::getInstance();

	//Lag::LogManager::getInstance().removeFlow(Lag::LogType::DEDUG, Lag::LogOutput::FILE);
	
	//Lag::LogManager::getInstance().addFlow(Lag::LogType::INFO, Lag::CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LogType::INFO, Lag::LogOutput::IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LogType::WARNING, Lag::LogOutput::CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LogType::WARNING, Lag::LogOutput::IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LogType::DEBUG, Lag::LogOutput::CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LogType::DEBUG, Lag::LogOutput::IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LogType::ERROR, Lag::LogOutput::CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LogType::ERROR, Lag::LogOutput::IDE);

	if (!root.initializeLag("startup.ini"))
		return false;

	TestScene *testScene1 = new TestScene(true);
	TestScene *testScene2 = new TestScene(false);

	//SceneManager will own the Scene objects
	root.getSceneManager().addScene("scene1", *testScene1); //will be auto-set as active
	root.getSceneManager().addScene("scene2", *testScene2);
	//root.getSceneManager().addScene("scene", *(new TestScene2()));

	root.startRenderingLoop();

	return true;
}