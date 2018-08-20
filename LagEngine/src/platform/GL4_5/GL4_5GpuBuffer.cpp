#include "GL4_5GpuBuffer.h"

using namespace Lag;

/*GL4_5GpuBuffer::GL4_5GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror) :
	GpuBuffer(sizeBytes, data, flags, contents, useMirror)
{
}*/

GL4_5GpuBuffer::GL4_5GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror) :
        GpuBuffer(sizeBytes, flags, contents, useMirror) {
}

void GL4_5GpuBuffer::lockImplementation(uint32 offset, uint32 length) {
}

void GL4_5GpuBuffer::unlockImplementation() {
}

byte *GL4_5GpuBuffer::mapImplementation() {
    GLbitfield usage = 0;
    if (checkForFlag(GpuBufferUsage::MAP_READ)) usage |= GL_MAP_READ_BIT;
    if (checkForFlag(GpuBufferUsage::MAP_WRITE)) usage |= GL_MAP_WRITE_BIT;

    void *ret;
    ret = glMapNamedBufferRange(handle, offsetLocked, lengthLocked, usage);
    return static_cast<byte *>(ret);
}

void GL4_5GpuBuffer::unmapImplementation() {
    glUnmapNamedBuffer(handle);
}

void GL4_5GpuBuffer::writeImplementation(uint32 offset, uint32 length, const byte *src) {
    uint32 realOffset = offsetLocked + offset;
    glNamedBufferSubData(handle, realOffset, length, static_cast<const GLvoid *>(src));
}

void GL4_5GpuBuffer::readImplementation(uint32 offset, uint32 length, byte *dst) {
    uint32 realOffset = offsetLocked + offset;
    glGetNamedBufferSubData(handle, realOffset, length, static_cast<GLvoid *>(dst));
}

bool Lag::GL4_5GpuBuffer::loadImplementation() {
    glCreateBuffers(1, &handle);
    glNamedBufferStorage(handle, sizeBytes, nullptr, convertFlagsToGL(flags));
    return GpuBuffer::loadImplementation();
}

void Lag::GL4_5GpuBuffer::unloadImplementation() {
    glDeleteBuffers(1, &handle);
    GpuBuffer::unloadImplementation();
}

GLbitfield GL4_5GpuBuffer::convertFlagsToGL(uint32 flags) {
    GLbitfield res = 0;
    if (checkForFlag(GpuBufferUsage::DYNAMIC)) res |= GL_DYNAMIC_STORAGE_BIT;
    if (checkForFlag(GpuBufferUsage::MAP_READ)) res |= GL_MAP_READ_BIT;
    if (checkForFlag(GpuBufferUsage::MAP_WRITE)) res |= GL_MAP_WRITE_BIT;
    return res;
}