#pragma once

namespace Lag
{
	class RenderQueue;
	
	/*
	* A Renderable should be able to provide RenderOperations to a RenderQueue.
	*/
	class Renderable
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual void addToRenderQueue(RenderQueue &renderQueue) = 0;
	};
}