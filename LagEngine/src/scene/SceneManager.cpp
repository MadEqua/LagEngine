#include "SceneManager.h"

#include "Scene.h"
#include "../Root.h"

using namespace Lag;

SceneManager::SceneManager() :
	currentScene(nullptr)
{
}

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const
{
	currentScene->addRenderablesToQueue(renderQueue, viewport, renderTarget);
}

Scene& SceneManager::createScene(const std::string &name)
{
	Scene *scene = new Scene();
	sceneMap[name] = scene;
	return *scene;
}

void SceneManager::setCurrentScene(const std::string &name)
{
	if (currentScene != nullptr)
	{
		currentScene->onEnd();
		Root::getInstance().clearUnusedResources();
	}

	auto it = sceneMap.find(name);
	if (it != sceneMap.end())
	{
		currentScene = it->second;
		currentScene->onStart();
	}
}