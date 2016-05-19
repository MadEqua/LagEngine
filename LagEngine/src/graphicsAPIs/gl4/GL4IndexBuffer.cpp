#include "GL4IndexBuffer.h"

using namespace Lag;

GL4IndexBuffer::GL4IndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirror) :
	IndexBuffer(indexCount, indexSizeBytes, useMirror)
{
}

GL4IndexBuffer::~GL4IndexBuffer()
{
}

void GL4IndexBuffer::writeImplementation(uint32 offset, uint32 length, byte* src)
{
}

void GL4IndexBuffer::readImplementation(uint32 offset, uint32 length, byte* dst)
{
}


void GL4IndexBuffer::lockImplementation(uint32 offset, uint32 length)
{
}

void GL4IndexBuffer::unlockImplementation()
{
}

byte* GL4IndexBuffer::mapImplementation()
{
	return nullptr;
}

void GL4IndexBuffer::unmapImplementation()
{
}