#pragma once

#include "../renderer/IRenderable.h"

namespace Lag
{
	class Material;
	class SubMesh;
	class RenderQueue;
	
	class SubEntity : public IRenderable
	{
		friend class Entity;
	
	private:
		SubEntity(Material &material, SubMesh &subMesh);
		~SubEntity();

		virtual void addToRenderQueue(RenderQueue &renderQueue) override;
		virtual void render(Renderer &renderer, uint32 passId) override;

	private:
		Material &material;
		SubMesh &subMesh;
	};
}