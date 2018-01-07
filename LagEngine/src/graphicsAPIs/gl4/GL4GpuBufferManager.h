#pragma once

#include "../../renderer/graphicsAPI/GpuBufferManager.h"
#include "GL4GpuBuffer.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4GpuBufferManager : public GpuBufferManager
	{
	public:
		virtual GpuBuffer* internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) override
		{
			return new GL4GpuBuffer(vertexCount * vertexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
		}

		/*virtual GpuBuffer* internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override
		{
			return new GL4GpuBuffer(vertexCount * vertexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
		}*/


		virtual GpuBuffer* internalCreateIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, uint32 flags, bool useMirrorBuffer) override
		{
			return new GL4GpuBuffer(indexCount * indexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
		}

		/*virtual GpuBuffer* internalCreateIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override
		{
			return new GL4GpuBuffer(indexCount * indexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
		}*/
	};
}