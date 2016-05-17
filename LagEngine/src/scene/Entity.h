#pragma once

#include <string>
#include "SceneObject.h"

namespace Lag
{
	class Material;
	class Mesh;
	
	/*
	* Object represented by a 3d mesh and a material.
	* Attach to a SceneNode to add to the scene.
	*/	
	class Entity : public SceneObject
	{
	public:
		Entity(Material &material, Mesh &mesh);
		//Entity(const std::string &meshName, const std::string &materialName);
		virtual ~Entity();

		inline Material& getMaterial() const { return material; }
		inline Mesh& getMesh() const { return mesh; }

	private:
		Material &material;
		Mesh &mesh;
	};
}