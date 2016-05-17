#pragma once

namespace Lag
{
	class SceneNode;
	
	class SceneObject 
	{
	public:
		SceneObject();
		virtual ~SceneObject();

		inline SceneNode* getParentSceneNode() const { return parentSceneNode; }
		inline bool isAttachedToSceneNode() const { return parentSceneNode != nullptr; }

		inline void attachToSceneNode(SceneNode& node) { parentSceneNode = &node; }
		inline void detachFromSceneNode() { parentSceneNode = nullptr; }

	protected:
		SceneNode *parentSceneNode;
	};
}
