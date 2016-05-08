#pragma once

#include "Material.h"
#include "Mesh.h"
#include "SceneObject.h"

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
		~Entity();

	private:
		Material material;
		Mesh mesh;
	};
}