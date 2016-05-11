#pragma once

#include <unordered_map>
#include <string>

namespace Lag
{
	class SceneNode;
	class SceneObject;

	/*
	* Hierarchical representation of a scene.
	* Responsible for allocation and deallocation of all the SceneNodes.
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

	private:
		SceneNode *root;

		//All created nodes by name
		std::unordered_map<std::string, SceneNode*> nodes;
	};
}