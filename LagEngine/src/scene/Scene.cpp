#include "Scene.h"

#include <algorithm>

#include "Handle.h"
#include "SceneManager.h"
#include "LogManager.h"
#include "Sky.h"
#include "Root.h"
#include "RenderTargetManager.h"
#include "RenderWindow.h"
#include "Entity.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AxisGizmo.h"

using namespace Lag;

Scene::Scene() :
        sceneGraph(*this),
        sky(nullptr),
        shouldChangeScene(false) {
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
    auto e = new Entity(meshName, materialName);
    sceneObjectMap.add(e);
    entityVector.push_back(e);
    renderableVector.push_back(e);
    return e;
}

Entity *Scene::createAxisGizmo() {
    auto gizmo = new AxisGizmo();
    sceneObjectMap.add(gizmo);
    entityVector.push_back(gizmo);
    renderableVector.push_back(gizmo);
    return gizmo;
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
