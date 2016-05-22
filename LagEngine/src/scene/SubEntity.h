#pragma once

#include "../renderer/Renderable.h"

namespace Lag
{
	class Material;
	class SubMesh;
	class RenderQueue;
	
	class SubEntity : public Renderable
	{
		friend class Entity;
	
	private:
		SubEntity(Material &material, SubMesh &subMesh);
		~SubEntity();

		virtual void addToRenderQueue(RenderQueue &renderQueue) override;

	private:
		Material &material;
		SubMesh &subMesh;
	};
}