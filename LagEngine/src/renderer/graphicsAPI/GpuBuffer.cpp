#include "GpuBuffer.h"
#include "../../core/MemoryBuffer.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuBuffer::GpuBuffer(uint32 sizeBytes, bool useMirror) :
	Buffer(sizeBytes),
	useMirror(useMirror),
	mirrorBuffer(nullptr)
{
	if (useMirror)
		mirrorBuffer = new MemoryBuffer(sizeBytes);
}

GpuBuffer::~GpuBuffer() 
{
	if (useMirror)
		delete mirrorBuffer;
}

void GpuBuffer::lock(uint32 offset, uint32 length)
{
	if (useMirror)
	{
		offsetLocked = offset;
		lengthLocked = length;
		return mirrorBuffer->lock(offset, length);
	}
	else
	{
		return this->lock(offset, length);
	}
}

void GpuBuffer::lock()
{
	return lock(0, sizeBytes);
}

void GpuBuffer::unlock()
{
	if (useMirror)
	{
		mirrorBuffer->unlock();

		//assuming writes happened. TODO: have an option to say the type of access on locking
		updateFromMirror();
	}
	else
	{
		this->unlock();
	}
}

byte* GpuBuffer::map()
{
	if (useMirror)
	{
		return mirrorBuffer->map();
	}
	else
	{
		return this->map();
	}
}

void GpuBuffer::unmap()
{
	if (useMirror)
	{
		mirrorBuffer->unmap();
	}
	else
	{
		this->unmap();
	}
}

void GpuBuffer::write(uint32 offset, uint32 length, byte* src)
{
	if (useMirror)
	{
		mirrorBuffer->write(offset, length, src);
	}
	else
		this->write(offset, length, src);
}

void GpuBuffer::read(uint32 offset, uint32 length, byte* dst)
{
	if (useMirror)
	{
		mirrorBuffer->read(offset, length, dst);
	}
	else
		this->read(offset, length, dst);
}


void GpuBuffer::updateFromMirror()
{
	if (useMirror)
	{
		mirrorBuffer->lock(offsetLocked, lengthLocked);
		this->lock(offsetLocked, lengthLocked);

		byte *src = mirrorBuffer->map();
		byte *dst = this->map();

		memcpy(dst, src, lengthLocked);

		this->unlock();
		mirrorBuffer->unlock();
	}
}