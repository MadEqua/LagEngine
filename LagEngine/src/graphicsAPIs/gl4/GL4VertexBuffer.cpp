#include "GL4VertexBuffer.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GL4VertexBuffer::GL4VertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirror) :
	VertexBuffer(vertexCount, vertexSizeBytes, useMirror)
{
}

GL4VertexBuffer::~GL4VertexBuffer()
{
}

void GL4VertexBuffer::writeImplementation(uint32 offset, uint32 length, byte* src)
{
}

void GL4VertexBuffer::readImplementation(uint32 offset, uint32 length, byte* dst)
{
}


void GL4VertexBuffer::lockImplementation(uint32 offset, uint32 length)
{
}

void GL4VertexBuffer::unlockImplementation()
{
}

byte* GL4VertexBuffer::mapImplementation()
{
	return nullptr;
}

void GL4VertexBuffer::unmapImplementation()
{
}