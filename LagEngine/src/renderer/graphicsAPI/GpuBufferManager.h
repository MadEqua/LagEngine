#pragma once

#include <vector>
#include "../../core/Manager.h"
#include "GpuBuffer.h"
#include "../../Types.h"

namespace Lag
{	
	class GpuBuffer;
	
	/*
	* Creates and manages all GpuBuffers.
	*/
	class GpuBufferManager : public Manager<uint32, GpuBuffer>
	{
	public:
		GpuBufferManager();

		GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer);
		GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer);

		GpuBuffer* createIndexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer);
		GpuBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer);

	protected:
		uint32 nextName;
		uint32 getNextName();

		virtual GpuBuffer* internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) = 0;
		virtual GpuBuffer* internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) = 0;

		virtual GpuBuffer* internalCreateIndexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) = 0;
		virtual GpuBuffer* internalCreateIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) = 0;
	};
}