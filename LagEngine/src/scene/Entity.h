#pragma once

#include "../renderer/Material.h"
#include "../renderer/Mesh.h"
#include "../scene/SceneObject.h"

namespace Lag
{
	/*
	* Object that can be rendered. Represented by a 3d mesh and a material.
	* Attach to a SceneNode to add to the scene.
	*/	
	class Entity : public SceneObject
	{
	public:
		Entity();
		virtual ~Entity();

	private:
		Material material;
		Mesh mesh;
	};
}