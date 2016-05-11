#pragma once

#include <unordered_map>
#include <string>

#include "SceneGraph.h"

namespace Lag
{
	class Camera;
	class Entity;
	
	/*
	* Contains all structures that represent a Scene for different purposes (object hierarchy, culling...)
	* Can also create and manage (for convenience) Entities, Cameras, Lights, ...
	*/
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		Entity& createEntity(const std::string &meshFileName);
		Camera& createCamera(const std::string &name);

		SceneObject* getSceneObject(const std::string &name);

		inline SceneGraph& getSceneGraph() { return sceneGraph; }

	private:
		SceneGraph sceneGraph;

		//TODO: is this ok for fast, linear iteration??
		std::unordered_map<std::string, SceneObject*> sceneObjects;

		//TODO spatial graph
	};
}