#include <memory>

#include "Scene.h"

#include <algorithm>

#include "Handle.h"
#include "SceneManager.h"
#include "LogManager.h"
#include "Root.h"
#include "RenderTargetManager.h"
#include "RenderWindow.h"
#include "Entity.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "MeshManager.h"

using namespace Lag;

Scene::Scene() :
        sceneGraph(*this),
        shouldChangeScene(false),
        isSkyEnabled(false) {
}

void Scene::start() {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Scene", "Scene starting.");
    onStart();
}

void Scene::initializeViewports() {
    Root &root = Root::getInstance();
    RenderWindow *renderWindow = root.getRenderTargetManager().getRenderWindow();
    onInitializeViewports(*renderWindow);
}

void Scene::end() {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Scene", "Scene ending.");

    onEnd();

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
    auto e = new Entity(meshName, materialName);
    addEntity(e);
    return e;
}

Entity *Scene::createEntity(Handle<Mesh> mesh, const std::string &materialName) {
    auto e = new Entity(mesh, materialName);
    addEntity(e);
    return e;
}

Entity *Scene::createAxisGizmo() {
    auto &meshManager = Root::getInstance().getMeshManager();
    return createEntity(meshManager.getAxisGizmo(), "axisGizmoMaterial");
}

void Scene::addEntity(Entity *entity) {
    sceneObjectMap.add(entity);
    entityVector.push_back(entity);
    renderableVector.push_back(entity);
}

PerspectiveCamera &Scene::createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane) {
    auto *cam = new PerspectiveCamera(aspectRatio, fovy, nearPlane, farPlane);
    sceneObjectMap.add(cam);
    cameraVector.push_back(cam);
    return *cam;
}

OrthographicCamera &Scene::createOrthographicCamera(float left, float right, float bottom, float top,
                                                    float nearPlane, float farPlane) {
    auto *cam = new OrthographicCamera(left, right, bottom, top, nearPlane, farPlane);
    sceneObjectMap.add(cam);
    cameraVector.push_back(cam);
    return *cam;
}

PointLight &Scene::createPointLight(const Color &color, const glm::vec3 &attenuation, bool castShadow) {
    auto *pl = new PointLight(color, attenuation, castShadow);
    sceneObjectMap.add(pl);
    pointLightVector.push_back(pl);
    return *pl;
}

DirectionalLight &Scene::createDirectionalLight(const Color &color, const glm::vec3 &direction, bool castShadow) {
    auto *dl = new DirectionalLight(direction, color, castShadow);
    sceneObjectMap.add(dl);
    directionalLightVector.push_back(dl);
    return *dl;
}

void Scene::enableSky(const std::string &materialName) {
    if(!isSkyEnabled) {
        auto &meshManager = Root::getInstance().getMeshManager();
        createEntity(meshManager.getCubeInsides(), materialName);
        isSkyEnabled = true;
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
    for(Entity *e : entityVector)
        e->onFrameStart(timePassed);
}

void Scene::onFrameRenderingQueued(float timePassed) {
    for(Entity *e : entityVector)
        e->onFrameRenderingQueued(timePassed);
}

void Scene::onFrameEnd(float timePassed) {
    for(Entity *e : entityVector)
        e->onFrameEnd(timePassed);
}

void Scene::onKeyPress(int key, int modifier) {
    for(Entity *e : entityVector)
        e->onKeyPress(key, modifier);
}

void Scene::onKeyRelease(int key, int modifier) {
    for(Entity *e : entityVector)
        e->onKeyRelease(key, modifier);
}

void Scene::onKeyRepeat(int key, int modifier) {
    for(Entity *e : entityVector)
        e->onKeyRepeat(key, modifier);
}

void Scene::onCursorMove(int x, int y) {
    for(Entity *e : entityVector)
        e->onCursorMove(x, y);
}

void Scene::onButtonPressed(int x, int y, int button, int modifiers) {
    for(Entity *e : entityVector)
        e->onButtonPressed(x, y, button, modifiers);
}

void Scene::onButtonReleased(int x, int y, int button, int modifiers) {
    for(Entity *e : entityVector)
        e->onButtonReleased(x, y, button, modifiers);
}
