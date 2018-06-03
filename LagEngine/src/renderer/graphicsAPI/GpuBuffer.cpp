#include "GpuBuffer.h"

#include "MemoryBuffer.h"
#include "LogManager.h"

using namespace Lag;

/*GpuBuffer::GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror) :
	Buffer(sizeBytes),
	useMirror(useMirror),
	mirrorBuffer(nullptr),
	flags(flags),
	contents(contents)
{
	dataCopy = new byte[sizeBytes];
	memcpy(dataCopy, data, sizeBytes);
}*/

GpuBuffer::GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror) :
        Buffer(sizeBytes),
        useMirror(useMirror),
        mirrorBuffer(nullptr),
        flags(flags),
        contents(contents) {
}

void GpuBuffer::lock(uint32 offset, uint32 length) {
    if (!checkForFlag(GpuBufferUsage::DYNAMIC)) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GpuBuffer", "Trying to lock non-dynamic GpuBuffer.");
        return;
    }

    if (useMirror) {
        offsetLocked = offset;
        lengthLocked = length;
        return mirrorBuffer->lock(offset, length);
    }
    else
        return this->Buffer::lock(offset, length);
}

void GpuBuffer::lock() {
    return lock(0, sizeBytes);
}

void GpuBuffer::unlock() {
    if (useMirror) {
        mirrorBuffer->unlock();

        if (checkForFlag(GpuBufferUsage::DYNAMIC))
            updateFromMirror();
    }
    else
        this->Buffer::unlock();
}

byte *GpuBuffer::map() {
    if (!checkForFlag(GpuBufferUsage::MAP_WRITE) &&
        !checkForFlag(GpuBufferUsage::MAP_READ)) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GpuBuffer", "Trying to map non-mappable GpuBuffer.");
        return nullptr;
    }

    if (useMirror)
        return mirrorBuffer->map();
    else
        return this->Buffer::map();
}

void GpuBuffer::unmap() {
    if (useMirror)
        mirrorBuffer->unmap();
    else
        this->Buffer::unmap();
}

void GpuBuffer::write(uint32 offset, uint32 length, byte *src) {
    if (useMirror)
        mirrorBuffer->write(offset, length, src);
    else
        this->Buffer::write(offset, length, src);
}

void GpuBuffer::read(uint32 offset, uint32 length, byte *dst) {
    if (useMirror)
        mirrorBuffer->read(offset, length, dst);
    else
        this->Buffer::read(offset, length, dst);
}

void GpuBuffer::updateFromMirror() {
    if (useMirror) {
        mirrorBuffer->lock(offsetLocked, lengthLocked);
        this->Buffer::lock(offsetLocked, lengthLocked);

        byte *src = mirrorBuffer->map();
        this->Buffer::write(0, lengthLocked, src);

        this->Buffer::unlock();
        mirrorBuffer->unlock();
    }
}

bool GpuBuffer::checkForFlag(GpuBufferUsage flagToCheck) {
    return static_cast<bool>(flags & static_cast<uint32>(flagToCheck));
}

bool Lag::GpuBuffer::loadImplementation() {
    if (useMirror)
        mirrorBuffer = new MemoryBuffer(sizeBytes);

    return true;
}

void Lag::GpuBuffer::unloadImplementation() {
    if (useMirror)
        delete mirrorBuffer;
}

void GpuBuffer::setUseMirror(bool use) {
    if (use && !useMirror)
        mirrorBuffer = new MemoryBuffer(sizeBytes);
    else if (!use && useMirror)
        delete mirrorBuffer;

    useMirror = use;
}
