#include "SceneManager.h"

#include "../io/log/LogManager.h"
#include "Camera.h"

using namespace Lag;

SceneManager::SceneManager()
{
	LogManager::getInstance().log(FILE, NORMAL, INFO, "SceneManager", "Initialized successfully.");
}


SceneManager::~SceneManager()
{
	for (auto &obj : sceneObjects)
		delete obj.second;

	LogManager::getInstance().log(FILE, NORMAL, INFO, "SceneManager", "Destroyed successfully.");
}


/*Entity& SceneManager::createEntity(const std::string &meshFileName)
{

}*/

Camera& SceneManager::createCamera(const std::string &name)
{
	Camera *cam = new Camera();
	sceneObjects[name] = cam;
	return *cam;
}

SceneObject* SceneManager::getSceneObject(const std::string &name)
{
	auto it = sceneObjects.find(name);
	if (it != sceneObjects.end())
		return it->second;
	else
		return nullptr;
}