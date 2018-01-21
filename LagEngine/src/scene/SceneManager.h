#pragma once

#include <unordered_map>
#include <string>

#include "Scene.h"

namespace Lag
{
	//TODO: good scene management with multiple scenes
	class SceneManager
	{
	public:
		SceneManager();

		//Fill a RenderQueue with objects in range of the Camera of the received Viewport
		void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const;

		Scene& createScene(const std::string &name);
		void setCurrentScene(const std::string &name);

		inline Scene& getCurrentScene() const { return *currentScene; }

	private:
		Scene *currentScene;
		std::unordered_map<std::string, Scene*> sceneMap;
	};
}