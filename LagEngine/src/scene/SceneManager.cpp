#include "SceneManager.h"

#include "Scene.h"
#include "../io/log/LogManager.h"

using namespace Lag;

SceneManager::SceneManager(const std::string &folder) :
	XmlResourceManager("SceneManager", folder),
	currentScene(nullptr)
{
}

void SceneManager::addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const
{
	currentScene->addRenderablesToQueue(renderQueue, viewport, renderTarget);
}

bool SceneManager::create(const std::string &name, const std::string &file)
{
	return add(name, new Scene(folder + '/' + file));
}

void SceneManager::setCurrentScene(const std::string &name)
{
	auto &it = objects.find(name);

	if (it == objects.end())
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"SceneManager", "There is no scene with the name: " + name);
	else if (currentScene == it->second)
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"SceneManager", "Scene " + name + " is already the current Scene.");
	else
	{
		if (currentScene != nullptr)
			currentScene->unload(); //TODO: need the name to unload through the manager
		
		if(load(name))
			currentScene = it->second;
	}
}

void SceneManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "scene")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");
		
		if (!name || !file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "SceneManager",
				"A <scene> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		create(name, file);

		/*bool def = false;
		element.QueryBoolAttribute("default", &def);

		if(def)
			setCurrentScene(name);*/
	}
}