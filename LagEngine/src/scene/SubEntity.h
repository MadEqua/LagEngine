#pragma once

#include "../renderer/IRenderable.h"

namespace Lag
{
	class Material;
	class SubMesh;
	class RenderQueue;
	class Entity;
	struct RenderOperation;
	
	class SubEntity : public IRenderable
	{
		friend class Entity;
	
	private:
		SubEntity(Entity &parent, Material &material, SubMesh &subMesh);

		virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
		virtual void render(Renderer &renderer, RenderOperation &renderOperation) override;

	private:
		Material &material;
		SubMesh &subMesh;
		Entity &parent;
	};
}