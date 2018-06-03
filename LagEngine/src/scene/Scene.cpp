#include "Scene.h"

#include <algorithm>

#include "Handle.h"
#include "SceneManager.h"
#include "LogManager.h"
#include "Sky.h"
#include "Root.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "RenderTargetManager.h"
#include "RenderWindow.h"
#include "Entity.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "PointLight.h"
#include "DirectionalLight.h"

using namespace Lag;

Scene::Scene() :
        sky(nullptr),
        shouldChangeScene(false) {
}

void Scene::start() {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                  "Scene", "Scene starting.");

    onStart();
}

void Scene::initializeViewports() {
    Root &root = Root::getInstance();
    RenderWindow *renderWindow = root.getRenderTargetManager().getRenderWindow();
    onInitializeViewports(*renderWindow);
}

void Scene::end() {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                  "Scene", "Scene ending.");

    onEnd();

    disableSky();

    sceneGraph.clear();

    //Memory deallocations happens here, on the main SceneObject repository.
    sceneObjectMap.clear();

    entityVector.clear();
    cameraVector.clear();
    pointLightVector.clear();
    directionalLightVector.clear();
    renderableVector.clear();
}

Entity *Scene::createEntity(const std::string &meshName, const std::string &materialName) {
    Root &root = Root::getInstance();

    Handle<Mesh> mesh = root.getMeshManager().get(meshName);
    Handle<Material> material = root.getMaterialManager().get(materialName);

    if (!mesh.isValid() || !material.isValid()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Scene",
                                      "Trying to build an Entity with invalid Mesh or Material: " + meshName + ", " +
                                      materialName);
        return nullptr;
    }

    Entity *e = new Entity(sceneObjectMap.getNextName(), material, mesh);
    sceneObjectMap.add(e);
    entityVector.push_back(e);
    renderableVector.push_back(e);
    return e;
}

PerspectiveCamera &Scene::createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane) {
    auto *cam = new PerspectiveCamera(sceneObjectMap.getNextName(), aspectRatio, fovy, nearPlane, farPlane);
    sceneObjectMap.add(cam);
    cameraVector.push_back(cam);
    return *cam;
}

OrthographicCamera &Scene::createOrthographicCamera(float left, float right, float bottom, float top,
                                                    float nearPlane, float farPlane) {
    auto *cam = new OrthographicCamera(sceneObjectMap.getNextName(), left, right, bottom, top, nearPlane, farPlane);
    sceneObjectMap.add(cam);
    cameraVector.push_back(cam);
    return *cam;
}

PointLight &Scene::createPointLight(const Color &color, const glm::vec3 &attenuation, bool castShadow) {
    auto *pl = new PointLight(sceneObjectMap.getNextName(), color, attenuation, castShadow);
    sceneObjectMap.add(pl);
    pointLightVector.push_back(pl);
    return *pl;
}

DirectionalLight &Scene::createDirectionalLight(const Color &color, const glm::vec3 &direction, bool castShadow) {
    auto *dl = new DirectionalLight(sceneObjectMap.getNextName(), direction, color, castShadow);
    sceneObjectMap.add(dl);
    directionalLightVector.push_back(dl);
    return *dl;
}

void Scene::enableSky(const std::string &materialName) {
    if (sky != nullptr)
        disableSky();

    sky = new Sky(materialName);
    renderableVector.push_back(sky);
}

void Scene::disableSky() {
    if (sky != nullptr) {
        renderableVector.erase(std::find(renderableVector.begin(), renderableVector.end(), sky));
        delete sky;
        sky = nullptr;
    }
}

SceneObject *Scene::getSceneObject(uint32 name) const {
    return sceneObjectMap.get(name);
}

void Scene::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const {
    //TODO: perform frustum culling with camera

    for (IRenderable *renderable : renderableVector)
        renderable->addToRenderQueue(renderQueue, viewport, renderTarget);
}

void Scene::scheduleSceneChange(const std::string &name) {
    shouldChangeScene = true;
    sceneToChange = name;
}

/////////////////////////////////
// Callbacks from SceneManager
/////////////////////////////////
void Scene::onFrameStart(float timePassed) {
}

void Scene::onFrameRenderingQueued(float timePassed) {
}

void Scene::onFrameEnd(float timePassed) {
    if (shouldChangeScene) {
        shouldChangeScene = false;
        Root::getInstance().getSceneManager().setActiveScene(sceneToChange);
    }
}

void Scene::onKeyPress(int key, int modifier) {
}

void Scene::onKeyRelease(int key, int modifier) {
}

void Scene::onKeyRepeat(int key, int modifier) {
}

void Scene::onCursorMove(int x, int y) {
}

void Scene::onButtonPressed(int x, int y, int button, int modifiers) {
}

void Scene::onButtonReleased(int x, int y, int button, int modifiers) {
}