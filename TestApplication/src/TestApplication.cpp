#include "TestApplication.h"

#include "Root.h"
#include "io/log/LogManager.h"
#include "scene/SceneManager.h"

#include "TestScene.h"


bool TestApplication::start()
{
	Lag::Root &root = Lag::Root::getInstance();

	//Lag::LogManager::getInstance().removeFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_FILE);
	
	//Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_INFO, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_INFO, Lag::LAG_LOG_OUT_IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_WARNING, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_WARNING, Lag::LAG_LOG_OUT_IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_DEBUG, Lag::LAG_LOG_OUT_IDE);

	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_CONSOLE);
	Lag::LogManager::getInstance().addFlow(Lag::LAG_LOG_TYPE_ERROR, Lag::LAG_LOG_OUT_IDE);

	if (!root.initializeLag("startup.ini"))
		return false;

	TestScene *testScene1 = new TestScene(true);
	TestScene *testScene2 = new TestScene(false);

	//SceneManager will own the Scene objects
	root.getSceneManager().addScene("scene1", *testScene1); //will be auto-set as active
	root.getSceneManager().addScene("scene2", *testScene2);

	root.startRenderingLoop();

	return true;
}