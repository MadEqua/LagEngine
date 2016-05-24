#pragma once

#include <vector>
#include "../../Types.h"

namespace Lag
{	
	class VertexBuffer;
	class IndexBuffer;
	class VertexDescription;
	
	/*
	* Creates and manages all GpuBuffers. Also creates and manages VertexDescriptions.
	*/
	class GpuBufferManager
	{
	public:
		GpuBufferManager();
		~GpuBufferManager();

		virtual VertexBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer) = 0;
		virtual IndexBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer) = 0;

		//TODO: make it so that it does not create similar descriptions...
		VertexDescription& createVertexDescription();

	protected:
		std::vector<VertexBuffer*> vertexBuffers;
		std::vector<VertexDescription*> vertexDescriptions;

		std::vector<IndexBuffer*> indexBuffers;
	};
}