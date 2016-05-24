#pragma once

#include <vector>
#include "../../Types.h"

namespace Lag
{	
	class GpuBuffer;
	
	/*
	* Creates and manages all GpuBuffers.
	*/
	class GpuBufferManager
	{
	public:
		GpuBufferManager();
		~GpuBufferManager();

		virtual GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) = 0;
		virtual GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) = 0;

		virtual GpuBuffer* createIndexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) = 0;
		virtual GpuBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) = 0;

	protected:
		std::vector<GpuBuffer*> vertexBuffers;
		std::vector<GpuBuffer*> indexBuffers;
	};
}