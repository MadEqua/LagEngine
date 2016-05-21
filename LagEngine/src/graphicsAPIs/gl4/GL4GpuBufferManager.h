#pragma once

#include "../../renderer/graphicsAPI/GpuBufferManager.h"
#include "GL4VertexBuffer.h"
#include "GL4IndexBuffer.h"

namespace Lag
{
	class GL4GpuBufferManager : public GpuBufferManager
	{
	public:
		GL4GpuBufferManager() {}
		virtual ~GL4GpuBufferManager() {}

		virtual VertexBuffer* createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer) override
		{
			VertexBuffer *vb = new GL4VertexBuffer(vertexCount, vertexSizeBytes, useMirrorBuffer);
			vertexBuffers.push_back(vb);
			return vb;
		}

		virtual IndexBuffer* createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer) override
		{
			IndexBuffer *ib = new GL4IndexBuffer(indexCount, indexSizeBytes, useMirrorBuffer);
			indexBuffers.push_back(ib);
			return ib;
		}
	};
}