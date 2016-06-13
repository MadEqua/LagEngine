#pragma once

#include "../Types.h"

namespace Lag
{
	class VertexData;
	class IndexData;
	class Material;
	class IRenderable;
	class Viewport;
	enum RenderPhase;
	
	/*
	* A RenderOperation represents a single render call to the graphics API. 
	* Renderables provide this data to the RenderQueue, 
	* which it uses for sorting and posterior render call dispatching.
	*/
	class RenderOperation
	{
	public:
		RenderOperation() {}
		~RenderOperation() {}

		RenderPhase renderPhase;
		VertexData *vertexData;
		IndexData *indexData;
		Material *material;
		IRenderable *renderable;
		Viewport *viewport;
		uint32 passId;
	};
}