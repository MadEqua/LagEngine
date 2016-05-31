#include "SceneManager.h"

#include "../io/log/LogManager.h"

#include "../Root.h"
#include "../resources/MeshManager.h"
#include "../resources/MaterialManager.h"

#include "Entity.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "../renderer/IRenderable.h"

using namespace Lag;

SceneManager::SceneManager()
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"SceneManager", "Initialized successfully.");
}


SceneManager::~SceneManager()
{
	for (auto &pair : sceneObjectMap)
		delete pair.second;

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"SceneManager", "Destroyed successfully.");
}


Entity* SceneManager::createEntity(const std::string &name, const std::string &meshName, const std::string &materialName)
{
	Root &root = Root::getInstance();

	Mesh *mesh = reinterpret_cast<Mesh*>(root.getMeshManager().get(meshName));
	Material *material = reinterpret_cast<Material*>(root.getMaterialManager().get(materialName));

	if (!mesh || !material)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
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

PointLight& SceneManager::createPointLight(const std::string &name, const Color& color, const glm::vec3 &attenuation)
{
	PointLight *pl = new PointLight(color, attenuation);
	sceneObjectMap[name] = pl;
	pointLightVector.push_back(pl);
	return *pl;
}

DirectionalLight& SceneManager::createDirectionalLight(const std::string &name, 
	const Color& color, const glm::vec3& direction)
{
	DirectionalLight *dl = new DirectionalLight(direction, color);
	sceneObjectMap[name] = dl;
	directionalLightVector.push_back(dl);
	return *dl;
}

SceneObject* SceneManager::getSceneObject(const std::string &name) const
{
	auto it = sceneObjectMap.find(name);
	if (it != sceneObjectMap.end())
		return it->second;
	else
		return nullptr;
}

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport) const
{
	//TODO: perform frustum culling with camera

	for (IRenderable *renderable : renderableVector)
		renderable->addToRenderQueue(renderQueue, viewport);
}