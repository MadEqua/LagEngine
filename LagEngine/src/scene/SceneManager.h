#pragma once

#include "Scene.h"
#include "../resources/XmlResourceManager.h"

namespace Lag
{
	class SceneManager : public XmlResourceManager<Scene>
	{
	public:
		explicit SceneManager(const std::string &folder);

		//Fill a RenderQueue with objects in range of the Camera of the received Viewport
		void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const;

		bool create(const std::string &name, const std::string &file);

		inline Scene& getCurrentScene() const { return *currentScene; }
		void setCurrentScene(const std::string &name);

	private:
		virtual void parseResourceDescription(const TiXmlElement &element);
		
		Scene *currentScene;
	};
}