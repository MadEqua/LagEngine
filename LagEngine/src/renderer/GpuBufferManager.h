#pragma once

#include <vector>
#include "../Types.h"
#include "../renderer/GraphicsApiType.h"

namespace Lag
{	
	class VertexBuffer;
	class IndexBuffer;
	class VertexBuffer;
	class VertexDescription;

	
	/*
	* Creates and manages all GpuBuffers. 
	* Also creates and manages VertexDescriptions.
	*/
	class GpuBufferManager
	{
	public:
		GpuBufferManager(GraphicsApiType graphicsApiType);
		~GpuBufferManager();

		VertexBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer);
		IndexBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer);

		VertexDescription& createVertexDescription();

	private:
		std::vector<VertexBuffer*> vertexBuffers;
		std::vector<VertexDescription*> vertexDescriptions;

		std::vector<IndexBuffer*> indexBuffers;

		GraphicsApiType graphicsApiType;
	};
}