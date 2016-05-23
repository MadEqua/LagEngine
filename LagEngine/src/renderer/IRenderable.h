#pragma once

#include "../Types.h"

namespace Lag
{
	class RenderQueue;
	class IGraphicsAPI;
	
	/*
	* A Renderable should be able to add itself to a RenderQueue and
	* know how to render itself by issuing calls to a IGraphicsAPI.
	*/
	class IRenderable
	{
	public:
		IRenderable() {}
		virtual ~IRenderable() {}

		virtual void addToRenderQueue(RenderQueue &renderQueue) = 0;
		virtual void render(IGraphicsAPI &graphicsAPI, uint32 passId) = 0;
	};
}