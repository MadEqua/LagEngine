#pragma once

#include <vector>
#include "../../Types.h"

namespace Lag
{	
	class VertexBuffer;
	class IndexBuffer;
	
	/*
	* Creates and manages all GpuBuffers.
	*/
	class GpuBufferManager
	{
	public:
		GpuBufferManager();
		~GpuBufferManager();

		virtual VertexBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer) = 0;
		virtual IndexBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer) = 0;

	protected:
		std::vector<VertexBuffer*> vertexBuffers;
		std::vector<IndexBuffer*> indexBuffers;
	};
}