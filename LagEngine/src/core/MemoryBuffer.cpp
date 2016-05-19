#include "MemoryBuffer.h"
#include "../io/log/LogManager.h"
#include <iostream>

using namespace Lag;

MemoryBuffer::MemoryBuffer(uint32 sizeBytes) :
	Buffer(sizeBytes)
{
	data = new byte[sizeBytes];
}

MemoryBuffer::~MemoryBuffer()
{
	if (data != nullptr)
		delete[] data;
}

void MemoryBuffer::lockImplementation(uint32 offset, uint32 length)
{
}

void MemoryBuffer::unlockImplementation()
{
}

byte* Lag::MemoryBuffer::mapImplementation()
{
	return data + offsetLocked;
}

void Lag::MemoryBuffer::unmapImplementation()
{
}

void MemoryBuffer::writeImplementation(uint32 offset, uint32 length, byte* src)
{
	memcpy(data + offset, src, length);
}

void MemoryBuffer::readImplementation(uint32 offset, uint32 length, byte* dst)
{
	memcpy(dst, data + offset, length);
}


