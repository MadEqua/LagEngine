#pragma once

#include "GL4GpuBufferManager.h"

#include "GL4GpuBuffer.h"

using namespace Lag;

GpuBuffer* GL4GpuBufferManager::internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer)
{
	return new GL4GpuBuffer(vertexCount * vertexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
}

GpuBuffer* GL4GpuBufferManager::internalCreateVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer)
{
	return new GL4GpuBuffer(vertexCount * vertexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_VERTICES, useMirrorBuffer);
}

GpuBuffer* GL4GpuBufferManager::internalCreateIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, uint32 flags, bool useMirrorBuffer)
{
	return new GL4GpuBuffer(indexCount * indexSizeBytes, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
}

GpuBuffer* GL4GpuBufferManager::internalCreateIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer)
{
	return new GL4GpuBuffer(indexCount * indexSizeBytes, data, flags, LAG_GPU_BUFFER_CONTENTS_INDICES, useMirrorBuffer);
}