#include "VertexBuffer.h"

using namespace Lag;

VertexBuffer::VertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirror) :
	GpuBuffer(vertexCount * vertexSizeBytes, useMirror),
	vertexCount(vertexCount)
{
	sizeBytes = vertexCount * vertexSizeBytes;
}

VertexBuffer::~VertexBuffer()
{
}