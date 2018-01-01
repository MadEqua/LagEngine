#pragma once

#include "../../renderer/graphicsAPI/GpuBufferManager.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4GpuBufferManager : public GpuBufferManager
	{
	public:
		virtual GpuBuffer& createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer) override;
		virtual GpuBuffer& createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override;

		virtual GpuBuffer& createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, uint32 flags, bool useMirrorBuffer) override;
		virtual GpuBuffer& createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer) override;
	};
}