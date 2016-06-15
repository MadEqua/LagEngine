#pragma once

#include <vector>
#include "SceneObject.h"
#include "../renderer/IRenderable.h"

namespace Lag
{
	class SubEntity;
	class Material;
	class Mesh;
	class RenderOperation;
	
	/*
	* Object represented by a Mesh (divided on SubMeshes) and a Material.
	* Each SubMesh of the received Mesh will be associated with a child SubEntity. 
	* This implies that there's always at least one SubEntity. 
	* Attach to a SceneNode to add to the scene.
	*/	
	class Entity : public SceneObject, public IRenderable
	{
	public:

		//defaultMaterial will be used if the meshes contain no material information
		Entity(uint32 name, Material &defaultMaterial, Mesh &mesh);
		virtual ~Entity();

		virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
		virtual void render(Renderer &renderer, RenderOperation &renderOperation) override;

	private:
		std::vector<SubEntity*> subEntities;

		Material &defaultMaterial;
		Mesh &mesh;
	};
}