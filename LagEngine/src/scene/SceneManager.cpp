#include "SceneManager.h"

#include "Root.h"
#include "LogManager.h"
#include "InputManager.h"
#include "RenderTargetManager.h"
#include "Renderer.h"

using namespace Lag;

SceneManager::SceneManager() :
        activeSceneName("") {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                  "Created successfully.");
}

SceneManager::~SceneManager() {
    if (hasActiveScene())
        getActiveScene().end();

    clear();

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                  "Destroyed successfully.");
}

void SceneManager::registerObservers() {
    //This will be registered through all the app lifecycle (it's a Manager), so we won't unregister
    Root &root = Root::getInstance();
    root.getRenderer().registerObserver(*this);
    root.getInputManager().registerObserver(static_cast<Lag::IKeyboardListener &>(*this));
    root.getInputManager().registerObserver(static_cast<Lag::ICursorListener &>(*this));
}

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const {
    if (hasActiveScene())
        getActiveScene().addRenderablesToQueue(renderQueue, viewport, renderTarget);
    else
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "SceneManager",
                                      "Trying to fill the Render Queue when there is not an active Scene.");
}

void SceneManager::addScene(const std::string &name, Scene &scene) {
    auto it = sceneMap.find(name);
    if (it != sceneMap.end()) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneManager",
                                      "Adding scene with already existent name: " + name + ". Replacing the older one.");
        removeScene(name);
    }

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                  "Added scene with name: " + name + ".");
    sceneMap[name] = &scene;

    if (!hasActiveScene()) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                      "Auto-setting the Scene " + name + " as the active Scene.");
        setActiveScene(name);
    }
}

void SceneManager::removeScene(const std::string &name) {
    auto it = sceneMap.find(name);
    if (it != sceneMap.end()) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                      "Removed scene with name: " + name + ".");
        delete it->second;
        sceneMap.erase(it);
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneManager",
                                      "Trying to remove a non-existent Scene: " + name + ".");
    }

    if (activeSceneName == name) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneManager",
                                      "Removing the active Scene: " + name + ".");
        activeSceneName = "";
    }
}

Scene &SceneManager::getActiveScene() const {
    if (!hasActiveScene())
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "SceneManager",
                                      "Trying to get the active Scene, but none is set.");

    return *sceneMap.at(activeSceneName);
}


void SceneManager::clear() {
    for (auto &pair : sceneMap)
        delete pair.second;

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                  "Cleared " + std::to_string(sceneMap.size()) + " Scenes.");

    sceneMap.clear();

    activeSceneName = "";
}

void SceneManager::setActiveScene(const std::string &name) {
    auto it = sceneMap.find(name);
    if (it == sceneMap.end()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "SceneManager",
                                      "Trying to set a non-existent Scene as active: " + name + ".");
        return;
    }

    if (activeSceneName == name) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SceneManager",
                                      "Setting the same active Scene as the currently active: " + name + ". Ignoring.");
        return;
    }

    if (hasActiveScene())
        getActiveScene().end();

    activeSceneName = name;

    Root::getInstance().getRenderTargetManager().resetToBasicState();
    Root::getInstance().getRenderer().resetToBasicState();

    sceneMap[name]->initializeViewports();
    sceneMap[name]->start();

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "SceneManager",
                                  "Active Scene is now " + name + ".");
}

///////////////////////////
// Callbacks
///////////////////////////
void SceneManager::onFrameStart(float timePassed) {
    if (hasActiveScene())
        getActiveScene().onFrameStart(timePassed);
}

void SceneManager::onFrameRenderingQueued(float timePassed) {
    if (hasActiveScene())
        getActiveScene().onFrameRenderingQueued(timePassed);
}

void SceneManager::onFrameEnd(float timePassed) {
    if (hasActiveScene())
        getActiveScene().onFrameEnd(timePassed);
}

void SceneManager::onKeyPress(int key, int modifier) {
    if (hasActiveScene())
        getActiveScene().onKeyPress(key, modifier);
}

void SceneManager::onKeyRelease(int key, int modifier) {
    if (hasActiveScene())
        getActiveScene().onKeyRelease(key, modifier);
}

void SceneManager::onKeyRepeat(int key, int modifier) {
    if (hasActiveScene())
        getActiveScene().onKeyRepeat(key, modifier);
}

void SceneManager::onCursorMove(int x, int y) {
    if (hasActiveScene())
        getActiveScene().onCursorMove(x, y);
}

void SceneManager::onButtonPressed(int x, int y, int button, int modifiers) {
    if (hasActiveScene())
        getActiveScene().onButtonPressed(x, y, button, modifiers);
}

void SceneManager::onButtonReleased(int x, int y, int button, int modifiers) {
    if (hasActiveScene())
        getActiveScene().onButtonReleased(x, y, button, modifiers);
}