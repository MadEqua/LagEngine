#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Lag
{
	class SceneNode;
	
	/*
	* A SceneObject is anything that can be attached to a SceneNode.
	*/
	class SceneObject 
	{
	public:
		SceneObject();
		virtual ~SceneObject();

		inline SceneNode* getParentSceneNode() const { return parentSceneNode; }
		inline bool isAttachedToSceneNode() const { return parentSceneNode != nullptr; }

		inline void attachToSceneNode(SceneNode& node) { parentSceneNode = &node; }
		inline void detachFromSceneNode() { parentSceneNode = nullptr; }

		//Convenience methods, transforms belong to SceneNodes
		const glm::vec3& getWorldPosition() const;

		const glm::mat4& getTransform() const;
		const glm::mat4& getInverseTransform() const;
		const glm::mat3& getNormalTransform() const;
		 
	protected:
		SceneNode *parentSceneNode;
		glm::mat4 tempMat4;
		glm::mat3 tempMat3;
		glm::vec3 tempVec;
	};
}
