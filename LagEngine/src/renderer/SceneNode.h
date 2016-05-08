#pragma once

#include <string>
#include <forward_list>

#include "Transform.h"

namespace Lag
{
	class SceneObject;
	class SceneGraph;
	
	/*
	* Nodes that form the SceneGraph hieararchy.
	* Contains a Transform that will be applied to all the attached SceneObjects.
	* SceneNodes can only be created through the SceneGraph.
	*/
	class SceneNode
	{
	public:

		SceneNode& createChildSceneNode(const std::string &name);
		void addChildSceneNode(const std::string &name);
		void addChildSceneNode(SceneNode &node);

		void removeChildSceneNode(const std::string &name);
		void removeChildSceneNode(SceneNode &node);

		void attachSceneObject(SceneObject &sceneObject);
		void detachSceneObject(SceneObject &sceneObject);

		inline bool hasParent() { return parent != nullptr; }
		inline SceneNode* getParentSceneNode() { return parent; }

	private:
		friend class SceneGraph;
		SceneNode(SceneGraph &sceneGraph);
		~SceneNode();

		Transform transform;
		SceneGraph &sceneGraph;
		SceneNode *parent; //May be nullptr when not added to the SceneGraph

		std::forward_list<SceneNode*> children;
		std::forward_list<SceneObject*> attachedObjects;
	};
}