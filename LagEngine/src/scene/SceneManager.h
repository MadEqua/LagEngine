#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "SceneGraph.h"

namespace Lag
{
	class Camera;
	class Entity;
	
	/*
	* Contains all structures that represent a Scene for different purposes (object hierarchy, culling, fast iteration, ...)
	* Can also create and manage (for convenience) Entities, Cameras, Lights, ...
	*/
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		Entity& createEntity(const std::string &meshFileName);
		Camera& createCamera(const std::string &name);

		SceneObject* getSceneObject(const std::string &name) const;

		//TODO: remove scene objects

		inline SceneGraph& getSceneGraph() { return sceneGraph; }
		inline const std::vector<Entity*>& getEntities() const { return entityVector; }

	private:
		SceneGraph sceneGraph;

		//All SceneObjects organized by name
		std::unordered_map<std::string, SceneObject*> sceneObjectMap;

		//SceneObjects organized by specific types
		std::vector<Entity*> entityVector;
		std::vector<Camera*> cameraVector;

		//TODO lightvector?

		//TODO spatial graph
	};
}