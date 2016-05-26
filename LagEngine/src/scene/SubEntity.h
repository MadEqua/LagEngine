#pragma once

#include "../renderer/IRenderable.h"

namespace Lag
{
	class Material;
	class SubMesh;
	class RenderQueue;
	class Entity;
	class RenderOperation;
	
	class SubEntity : public IRenderable
	{
		friend class Entity;
	
	private:
		SubEntity(Entity &parent, Material &material, SubMesh &subMesh);
		~SubEntity();

		virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport) override;
		virtual void render(Renderer &renderer, RenderOperation &renderOperation) override;

	private:
		Material &material;
		SubMesh &subMesh;
		Entity &parent;
	};
}