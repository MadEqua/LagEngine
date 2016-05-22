#pragma once

#include <vector>
#include "SceneObject.h"

namespace Lag
{
	class SubEntity;
	class Material;
	class Mesh;
	
	/*
	* Object represented by a Mesh (divided on SubMeshes) and a Material.
	* Each SubMesh of the received Mesh will be associated with a child SubEntity. 
	* This implies that there's always at least one SubEntity. 
	* Attach to a SceneNode to add to the scene.
	*/	
	class Entity : public SceneObject
	{
	public:

		//defaultMaterial will be used if the meshes contain no material information
		Entity(Material &defaultMaterial, Mesh &mesh);
		//Entity(const std::string &meshName, const std::string &materialName);
		virtual ~Entity();

		//inline Material& getMaterial() const { return material; }
		//inline Mesh& getMesh() const { return mesh; }

	private:
		std::vector<SubEntity*> subEntities;

		Material &defaultMaterial;
		Mesh &mesh;
	};
}