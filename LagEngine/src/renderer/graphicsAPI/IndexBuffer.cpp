#include "IndexBuffer.h"

using namespace Lag;

IndexBuffer::IndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirror) :
	GpuBuffer(indexCount * indexSizeBytes, useMirror),
	indexCount(indexCount)
{
}

IndexBuffer::~IndexBuffer()
{
}