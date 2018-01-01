#pragma once

#include "../Types.h"
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
		explicit SceneObject(uint32 name);
		virtual ~SceneObject() = default;

		inline uint32 getName() const { return name; }

		inline SceneNode* getParentSceneNode() const { return parentSceneNode; }
		inline bool isAttachedToSceneNode() const { return parentSceneNode != nullptr; }

		inline void attachToSceneNode(SceneNode& node) { parentSceneNode = &node; }
		inline void detachFromSceneNode() { parentSceneNode = nullptr; }

		inline SceneNode* getParentSceneNode() { return parentSceneNode; }

		//Convenience methods, transforms belong to SceneNodes
		const glm::vec3& getWorldPosition() const;

		const glm::mat4& getTransform() const;
		const glm::mat4& getInverseTransform() const;
		const glm::mat3& getNormalTransform() const;
		 
	protected:
		uint32 name;

		SceneNode *parentSceneNode;
		glm::mat4 tempMat4;
		glm::mat3 tempMat3;
		glm::vec3 tempVec;
	};
}
