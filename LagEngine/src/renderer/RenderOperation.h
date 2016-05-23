#pragma once

#include "../Types.h"

namespace Lag
{
	//TODO: check the possibilities
	/*enum RenderCommand
	{
		LAG_RENDER_COMM_VERTICES,
		LAG_RENDER_COMM_INDEXED,
		LAG_RENDER_COMM_MULTI_VERTICES,
		LAG_RENDER_COMM_MULTI_INDEXED
	};

	//TODO: check the possibilities
	enum RenderType
	{
		LAG_RENDER_TYPE_TRIANGLES,
		LAG_RENDER_TYPE_LINES,
		LAG_RENDER_TYPE_POINTS
	};*/

	class VertexData;
	class IndexData;
	class Material;
	class IRenderable;
	
	/*
	* A RenderOperation represents a single render call to the graphics API. 
	* Renderables provide this data to the RenderQueue, 
	* which it uses for sorting and posterior render call dispatching.
	*/
	class RenderOperation
	{
	public:
		RenderOperation();
		~RenderOperation();

		VertexData *vertexData;
		IndexData *indexData;
		Material *material;
		IRenderable *renderable;
		uint32 passId;
	};
}