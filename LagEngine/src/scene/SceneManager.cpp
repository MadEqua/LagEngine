#include "SceneManager.h"

#include "../io/log/LogManager.h"

#include "../Root.h"
#include "../resources/MeshManager.h"
#include "../resources/MaterialManager.h"

#include "Entity.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "../renderer/IRenderable.h"
#include "Sky.h"

using namespace Lag;

SceneManager::SceneManager() : sky(nullptr)
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"SceneManager", "Initialized successfully.");
}


SceneManager::~SceneManager()
{
	if (sky != nullptr)
		delete sky;

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"SceneManager", "Destroyed successfully.");
}

Entity* SceneManager::createEntity(const std::string &meshName, const std::string &materialName)
{
	Root &root = Root::getInstance();

	Mesh *mesh = root.getMeshManager().get(meshName);
	Material *material = root.getMaterialManager().get(materialName);

	if (!mesh || !material)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"SceneManager", "Trying to create an Entity with a non-existent Mesh or Material: " + meshName + ", " + materialName);
		return nullptr;
	}
	
	Entity *e = new Entity(sceneObjectMap.getNextName(), *material, *mesh);
	sceneObjectMap.add(e);
	entityVector.push_back(e);
	renderableVector.push_back(e);
	return e;
}

PerspectiveCamera& SceneManager::createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane)
{
	PerspectiveCamera *cam = new PerspectiveCamera(sceneObjectMap.getNextName(), aspectRatio, fovy, nearPlane, farPlane);
	sceneObjectMap.add(cam);
	cameraVector.push_back(cam);
	return *cam;
}

OrthographicCamera& SceneManager::createOrthographicCamera(float left, float right, float bottom, float top, 
	float nearPlane, float farPlane)
{
	OrthographicCamera *cam = new OrthographicCamera(sceneObjectMap.getNextName(), left, right, bottom, top, nearPlane, farPlane);
	sceneObjectMap.add(cam);
	cameraVector.push_back(cam);
	return *cam;
}

PointLight& SceneManager::createPointLight(const Color& color, const glm::vec3 &attenuation, bool castShadow)
{
	PointLight *pl = new PointLight(sceneObjectMap.getNextName(), color, attenuation, castShadow);
	sceneObjectMap.add(pl);
	pointLightVector.push_back(pl);
	return *pl;
}

DirectionalLight& SceneManager::createDirectionalLight(const Color& color, const glm::vec3& direction, bool castShadow)
{
	DirectionalLight *dl = new DirectionalLight(sceneObjectMap.getNextName(), direction, color, castShadow);
	sceneObjectMap.add(dl);
	directionalLightVector.push_back(dl);
	return *dl;
}

void SceneManager::enableSky(const std::string &materialName)
{
	if (sky != nullptr) 
		disableSky();

	sky = new Sky(materialName);
	renderableVector.push_back(sky);
}

void SceneManager::disableSky()
{
	if (sky != nullptr)
	{
		renderableVector.erase(std::find(renderableVector.begin(), renderableVector.end(), sky));
		delete sky;
		sky = nullptr;
	}
}

SceneObject* SceneManager::getSceneObject(uint32 name) const
{
	return sceneObjectMap.get(name);
}

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const
{
	//TODO: perform frustum culling with camera

	for (IRenderable *renderable : renderableVector)
		renderable->addToRenderQueue(renderQueue, viewport, renderTarget);
}