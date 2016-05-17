#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "SceneGraph.h"

namespace Lag
{
	class Camera;
	class Entity;
	class Renderable;
	
	/*
	* Contains all structures that represent a Scene for different purposes (object hierarchy, culling, fast iteration, ...)
	* Can also create and manage Entities, Cameras, Lights, ...
	*/
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		//Entity& createEntity(const std::string &name, const std::string &meshName, const std::string &materialName);
		Camera& createCamera(const std::string &name);

		SceneObject* getSceneObject(const std::string &name) const;

		//TODO: remove scene objects

		inline SceneGraph& getSceneGraph() { return sceneGraph; }
		inline const std::vector<Entity*>& getEntities() const { return entityVector; }
		inline const std::vector<Camera*>& getCameras() const { return cameraVector; }
		inline const std::vector<Renderable*>& getRenderables() const { return renderableVector; }

	private:
		SceneGraph sceneGraph;

		//All SceneObjects organized by name. Main repository.
		std::unordered_map<std::string, SceneObject*> sceneObjectMap;

		//SceneObjects organized by specific types
		std::vector<Entity*> entityVector;
		std::vector<Camera*> cameraVector;

		//Renderable SceneObjects
		std::vector<Renderable*> renderableVector;

		//TODO lightvector?

		//TODO spatial graph
	};
}