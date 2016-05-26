#include "SceneManager.h"

#include "../io/log/LogManager.h"

#include "../Root.h"
#include "../resources/MeshManager.h"
#include "../resources/MaterialManager.h"

#include "Entity.h"
#include "Camera.h"
#include "../renderer/IRenderable.h"

using namespace Lag;

SceneManager::SceneManager()
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"SceneManager", "Initialized successfully.");
}


SceneManager::~SceneManager()
{
	for (auto &pair : sceneObjectMap)
		delete pair.second;

	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"SceneManager", "Destroyed successfully.");
}


Entity* SceneManager::createEntity(const std::string &name, const std::string &meshName, const std::string &materialName)
{
	Root &root = Root::getInstance();

	Mesh *mesh = reinterpret_cast<Mesh*>(root.getMeshManager().get(meshName));
	Material *material = reinterpret_cast<Material*>(root.getMaterialManager().get(materialName));

	if (!mesh || !material)
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR,
			"SceneManager", "Trying to create an Entity with a non-existent Mesh or Material: " + meshName + ", " + materialName);
		return nullptr;
	}
	
	Entity *e = new Entity(*material, *mesh);
	sceneObjectMap[name] = e;
	entityVector.push_back(e);
	renderableVector.push_back(e);
	return e;
}

Camera& SceneManager::createCamera(const std::string &name, float fovy, float nearPlane, float farPlane)
{
	Camera *cam = new Camera(fovy, nearPlane, farPlane);
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

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport)
{
	//TODO: perform frustum culling with camera

	for (IRenderable *renderable : renderableVector)
		renderable->addToRenderQueue(renderQueue, viewport);
}