#pragma once

#include "../Types.h"

namespace Lag
{
	class RenderQueue;
	class Renderer;
	class Material;
	class Viewport;
	class RenderOperation;
	
	/*
	* A Renderable should be able to add itself to a RenderQueue and
	* know how to render itself by issuing calls to the Renderer.
	*/
	class IRenderable
	{
	public:
		IRenderable() {}
		virtual ~IRenderable() {}

		//The Viewport is passed to the Renderable and should be placed on the RenderOperation added to the queue
		virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport) = 0;
		
		//Called when its time for the Renderable to render itself. 
		//The RenderOperation passed is the one added to the RenderQueue by this Renderable 
		virtual void render(Renderer &renderer, RenderOperation &renderOperation) = 0;
	};
}