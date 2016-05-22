#pragma once

namespace Lag
{
	//TODO: check the possibilities
	enum RenderCommand
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
	};

	class VertexData;
	class IndexData;
	class ShaderProgram;
	
	/*
	* A RenderOperation represents a single draw call to the graphics API. 
	* It should contain all needed data to draw something on a RenderTarget.
	* Renderables provide this data to the RenderQueue.
	*/
	class RenderOperation
	{
	public:
		RenderOperation();
		~RenderOperation();

		RenderCommand renderCommand;
		RenderType renderType;

		VertexData *vertexData;
		IndexData *indexData;
		ShaderProgram *shaderProgram;

		//TODO: textures, blending, other pipeline parameters...
	};
}