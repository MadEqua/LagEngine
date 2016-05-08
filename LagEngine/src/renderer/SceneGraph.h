#pragma once

#include <unordered_map>
#include <string>

namespace Lag
{
	class SceneNode;
	class Entity;
	class Camera;

	/*
	* Hierarchical representation of a scene.
	* Responsible for allocation and deallocation of all the SceneNodes.
	*
	* Can also create and manage (for convenience) Entities, Cameras, Lights, ...
	* 
	* TODO: optimize memory arrangment of the nodes, for traversal efficiency.
	*/
	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		SceneNode& createSceneNode(const std::string &name);

		SceneNode* getSceneNode(const std::string &name);
		inline SceneNode& getRootSceneNode() { return *root; }
				
		/*Entity& createEntity(const std::string &meshFileName);
		Camera& createCamera();*/

		void traverse();

	private:
		SceneNode *root;

		//All created nodes
		std::unordered_map<std::string, SceneNode*> nodeMap;
	};
}