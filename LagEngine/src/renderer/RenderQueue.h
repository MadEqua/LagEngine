#pragma once

#include <vector>
#include "RenderOperation.h"
#include "../Types.h"

namespace Lag
{
	enum RenderCommand;
	enum RenderType;
	class VertexData;
	class IndexData;
	class ShaderProgram;
	
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

		void addItem(RenderCommand renderCommand, RenderType renderType, 
			VertexData &vertexData, IndexData &indexData,
			ShaderProgram &shaderProgram);

		//which one??
		//void addItem(RenderOperation &renderOperation);
 
		void clear();
		void sort();

	private:

		uint32 actualSlot;

		//TODO: find out what's better: vector of pointers or actual Values.
		//Pointers are better for sorting, but worse for fast sequential access.
		std::vector<RenderOperation> queue;
	};
}