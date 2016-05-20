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
	class IndexBufferFactory;
	class VertexBufferFactory;

	
	/*
	* Creates and manages all GpuBuffers. 
	* Also creates and manages VertexDescriptions.
	* Needs factories for Vx and Idx Buffers, because those types are graphicsAPI dependent. 
	* Each implementation should provide this factories.
	*/
	class GpuBufferManager
	{
	public:
		GpuBufferManager(VertexBufferFactory *vxBufferFactory, IndexBufferFactory *idxBufferFactory);
		~GpuBufferManager();

		VertexBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer);
		IndexBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer);

		VertexDescription& createVertexDescription();

	private:
		std::vector<VertexBuffer*> vertexBuffers;
		std::vector<VertexDescription*> vertexDescriptions;

		std::vector<IndexBuffer*> indexBuffers;

		IndexBufferFactory *indexBufferFactory;
		VertexBufferFactory *vertexBufferFactory;
	};
}