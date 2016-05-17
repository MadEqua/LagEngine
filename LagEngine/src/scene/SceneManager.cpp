#include "SceneManager.h"

#include "../io/log/LogManager.h"

#include "Entity.h"
#include "Camera.h"

using namespace Lag;

SceneManager::SceneManager()
{
	LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::INFO,
		"SceneManager", "Initialized successfully.");
}


SceneManager::~SceneManager()
{
	for (auto &pair : sceneObjectMap)
		delete pair.second;

	LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::INFO,
		"SceneManager", "Destroyed successfully.");
}


/*Entity& SceneManager::createEntity(const std::string &name, const std::string &meshName, const std::string &materialName)
{
	Entity *e = new Entity(meshName, materialName);
	sceneObjectMap[name] = e;
	entityVector.push_back(e);
	//renderableVector.push_back(e);
	return *e;
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