#pragma once

namespace Lag
{
	
	/*
	* A Renderable should be able to provide a Renderer all the necessary
	* data to start a render call on the graphics pipeline.
	*/
	class Renderable
	{
	public:
		Renderable();
		~Renderable();
	};
}