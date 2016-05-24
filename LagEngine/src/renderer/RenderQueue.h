#pragma once

#include <vector>
#include "RenderOperation.h"
#include "../Types.h"

namespace Lag
{
	class VertexData;
	class IndexData;
	class Material;
	class IRenderable;
	class IGraphicsAPI;
	
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

		/*void addItem(RenderCommand renderCommand, RenderType renderType, 
			VertexData &vertexData, IndexData &indexData,
			ShaderProgram &shaderProgram);*/

		void addRenderOperation(IRenderable &renderable, uint32 passId,
			VertexData &vertexData, IndexData *indexData,
			Material &material);
		
		//which one??
		//void addItem(RenderOperation &renderOperation);

		void dispatchRenderOperations(IGraphicsAPI &graphicsAPI);
 
		void clear();
		void sort();

	private:

		uint32 actualSlot;

		//TODO: find out what's better: vector of pointers or actual Values.
		//Pointers are better for sorting, but worse for fast sequential access.
		std::vector<RenderOperation> queue;
	};
}