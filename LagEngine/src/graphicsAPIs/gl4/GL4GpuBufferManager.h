#pragma once

#include "../../renderer/graphicsAPI/GpuBufferManager.h"
#include "GL4GpuBuffer.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4GpuBufferManager : public GpuBufferManager
	{
	public:
		GL4GpuBufferManager() {}
		virtual ~GL4GpuBufferManager() {}

		virtual GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) override
		{
			GpuBuffer *vb = new GL4GpuBuffer(vertexCount * vertexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
			vertexBuffers.push_back(vb);
			return vb;
		}

		virtual GpuBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override
		{
			GpuBuffer *vb = new GL4GpuBuffer(vertexCount * vertexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
			vertexBuffers.push_back(vb);
			return vb;
		}

		virtual GpuBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, uint32 flags, bool useMirrorBuffer) override
		{
			GpuBuffer *ib = new GL4GpuBuffer(indexCount * indexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
			indexBuffers.push_back(ib);
			return ib;
		}

		virtual GpuBuffer* createIndexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override
		{
			GpuBuffer *vb = new GL4GpuBuffer(vertexCount * vertexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
			vertexBuffers.push_back(vb);
			return vb;
		}
	};
}