#include "Scene.h"

#include "../io/log/LogManager.h"
#include "Sky.h"
#include "../Root.h"
#include "../renderer/Mesh.h"
#include "../renderer/Material.h"
#include "../resources/MeshManager.h"
#include "../resources/MaterialManager.h"
#include "Entity.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "PointLight.h"
#include "DirectionalLight.h"

using namespace Lag;

Scene::Scene() :
	sky(nullptr)
{
}

Scene::~Scene()
{
	if (sky != nullptr)
		delete sky;
}

void Scene::onStart()
{
}

void Scene::onEnd()
{
}

Entity* Scene::createEntity(const std::string &meshName, const std::string &materialName)
{
	Root &root = Root::getInstance();

	Mesh *mesh = root.getMeshManager().get(meshName);
	Material *material = root.getMaterialManager().get(materialName);

	if (!mesh || !material)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Scene", "Trying to build an Entity with a non-existent Mesh or Material: " + meshName + ", " + materialName);
		return nullptr;
	}

	Entity *e = new Entity(sceneObjectMap.getNextName(), *material, *mesh);
	sceneObjectMap.add(e);
	entityVector.push_back(e);
	renderableVector.push_back(e);
	return e;
}

PerspectiveCamera& Scene::createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane)
{
	PerspectiveCamera *cam = new PerspectiveCamera(sceneObjectMap.getNextName(), aspectRatio, fovy, nearPlane, farPlane);
	sceneObjectMap.add(cam);
	cameraVector.push_back(cam);
	return *cam;
}

OrthographicCamera& Scene::createOrthographicCamera(float left, float right, float bottom, float top,
	float nearPlane, float farPlane)
{
	OrthographicCamera *cam = new OrthographicCamera(sceneObjectMap.getNextName(), left, right, bottom, top, nearPlane, farPlane);
	sceneObjectMap.add(cam);
	cameraVector.push_back(cam);
	return *cam;
}

PointLight& Scene::createPointLight(const Color& color, const glm::vec3 &attenuation, bool castShadow)
{
	PointLight *pl = new PointLight(sceneObjectMap.getNextName(), color, attenuation, castShadow);
	sceneObjectMap.add(pl);
	pointLightVector.push_back(pl);
	return *pl;
}

DirectionalLight& Scene::createDirectionalLight(const Color& color, const glm::vec3& direction, bool castShadow)
{
	DirectionalLight *dl = new DirectionalLight(sceneObjectMap.getNextName(), direction, color, castShadow);
	sceneObjectMap.add(dl);
	directionalLightVector.push_back(dl);
	return *dl;
}

void Scene::enableSky(const std::string &materialName)
{
	if (sky != nullptr)
		disableSky();

	sky = new Sky(materialName);
	renderableVector.push_back(sky);
}

void Scene::disableSky()
{
	if (sky != nullptr)
	{
		renderableVector.erase(std::find(renderableVector.begin(), renderableVector.end(), sky));
		delete sky;
		sky = nullptr;
	}
}

SceneObject* Scene::getSceneObject(uint32 name) const
{
	return sceneObjectMap.get(name);
}

void Scene::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const
{
	//TODO: perform frustum culling with camera

	for (IRenderable *renderable : renderableVector)
		renderable->addToRenderQueue(renderQueue, viewport, renderTarget);
}