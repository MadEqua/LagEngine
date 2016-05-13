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
	for (auto &pair : sceneObjectMap)
		delete pair.second;

	LogManager::getInstance().log(FILE, NORMAL, INFO, "SceneManager", "Destroyed successfully.");
}


/*Entity& SceneManager::createEntity(const std::string &meshFileName)
{

}*/

Camera& SceneManager::createCamera(const std::string &name)
{
	Camera *cam = new Camera();
	sceneObjectMap[name] = cam;
	cameraVector.push_back(cam);
	return *cam;
}

SceneObject* SceneManager::getSceneObject(const std::string &name) const
{
	auto it = sceneObjectMap.find(name);
	if (it != sceneObjectMap.end())
		return it->second;
	else
		return nullptr;
}