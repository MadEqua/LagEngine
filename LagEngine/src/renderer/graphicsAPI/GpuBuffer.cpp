	#include "GpuBuffer.h"
#include "../../core/MemoryBuffer.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuBuffer::GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror) :
	Buffer(sizeBytes),
	useMirror(useMirror),
	mirrorBuffer(nullptr),
	flags(flags),
	contents(contents)
{
	if (useMirror)
		mirrorBuffer = new MemoryBuffer(sizeBytes, data);
}

GpuBuffer::GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror) :
	Buffer(sizeBytes),
	useMirror(useMirror),
	mirrorBuffer(nullptr),
	flags(flags),
	contents(contents)
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
	if (!checkForFlag(LAG_GPU_BUFFER_USAGE_DYNAMIC))
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"GpuBuffer", "Trying to lock non-dynamic GpuBuffer.");
		return;
	}
	
	if (useMirror)
	{
		offsetLocked = offset;
		lengthLocked = length;
		return mirrorBuffer->lock(offset, length);
	}
	else
	{
		return this->Buffer::lock(offset, length);
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
		
		if (checkForFlag(LAG_GPU_BUFFER_USAGE_DYNAMIC))
			updateFromMirror();
	}
	else
	{
		this->Buffer::unlock();
	}
}

byte* GpuBuffer::map()
{
	if (!checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_WRITE) &&
		!!checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_READ))
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"GpuBuffer", "Trying to map non-mappable GpuBuffer.");
		return nullptr;
	}
	
	if (useMirror)
	{
		return mirrorBuffer->map();
	}
	else
	{
		return this->Buffer::map();
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
		this->Buffer::unmap();
	}
}

void GpuBuffer::write(uint32 offset, uint32 length, byte* src)
{
	if (useMirror)
	{
		mirrorBuffer->write(offset, length, src);
	}
	else
		this->Buffer::write(offset, length, src);
}

void GpuBuffer::read(uint32 offset, uint32 length, byte* dst)
{
	if (useMirror)
	{
		mirrorBuffer->read(offset, length, dst);
	}
	else
		this->Buffer::read(offset, length, dst);
}


void GpuBuffer::updateFromMirror()
{
	if (useMirror)
	{
		mirrorBuffer->lock(offsetLocked, lengthLocked);
		this->Buffer::lock(offsetLocked, lengthLocked);

		byte *src = mirrorBuffer->map();
		byte *dst = this->Buffer::map();

		memcpy(dst, src, lengthLocked);

		this->Buffer::unlock();
		mirrorBuffer->unlock();
	}
}

bool GpuBuffer::checkForFlag(GpuBufferUsage flagToCheck)
{
	return static_cast<bool>(flags & flagToCheck);
}