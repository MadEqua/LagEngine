#pragma once

#include "glm/glm.hpp"

namespace Lag
{
	class IndexBuffer;
	class VertexBuffer;
	class GpuProgram;
	
	/*
	* A Renderable should be able to provide the Renderer all the necessary
	* data to start a render operation on the graphics pipeline.
	*/
	class Renderable
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual VertexBuffer& getVertexBuffer() = 0;
		
		virtual bool isIndexed() = 0;
		virtual IndexBuffer& getIndexBuffer() = 0;

		virtual glm::mat4& getModelMatrix() = 0;

		//virtual GpuProgram& getGpuProgram() = 0;
	};
}