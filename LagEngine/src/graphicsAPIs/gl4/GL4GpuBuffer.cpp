#include "GL4GpuBuffer.h"

using namespace Lag;

GL4GpuBuffer::GL4GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror) :
	GpuBuffer(sizeBytes, data, flags, contents, useMirror)
{
	GL_ERROR_CHECK(glCreateBuffers(1, &handle))
	GL_ERROR_CHECK(glNamedBufferStorage(handle, sizeBytes, data, convertFlagsToGL(flags)))
}

GL4GpuBuffer::GL4GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror) :
	GpuBuffer(sizeBytes, flags, contents, useMirror)
{
	GL_ERROR_CHECK(glCreateBuffers(1, &handle))
	GL_ERROR_CHECK(glNamedBufferStorage(handle, sizeBytes, 0, convertFlagsToGL(flags)))
}

GL4GpuBuffer::~GL4GpuBuffer()
{
	GL_ERROR_CHECK(glDeleteBuffers(1, &handle))
}

void GL4GpuBuffer::lockImplementation(uint32 offset, uint32 length)
{
}

void GL4GpuBuffer::unlockImplementation()
{
}

byte* GL4GpuBuffer::mapImplementation()
{
	GLbitfield usage = 0;
	if (checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_READ)) usage |= GL_MAP_READ_BIT;
	if (checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_WRITE)) usage |= GL_MAP_WRITE_BIT;
	
	void *ret;
	GL_ERROR_CHECK(ret = glMapNamedBufferRange(handle, offsetLocked, lengthLocked, usage))
	return static_cast<byte*>(ret);
}

void GL4GpuBuffer::unmapImplementation()
{
	GL_ERROR_CHECK(glUnmapNamedBuffer(handle))
}

void GL4GpuBuffer::writeImplementation(uint32 offset, uint32 length, byte* src)
{
	uint32 realOffset = offsetLocked + offset;
	GL_ERROR_CHECK(glNamedBufferSubData(handle, realOffset, length, static_cast<GLvoid*>(src)))
}

void GL4GpuBuffer::readImplementation(uint32 offset, uint32 length, byte* dst)
{
	uint32 realOffset = offsetLocked + offset;
	GL_ERROR_CHECK(glGetNamedBufferSubData(handle, realOffset, length, static_cast<GLvoid*>(dst)));
}

void GL4GpuBuffer::bind() const
{
	switch (contents)
	{
	case LAG_GPU_BUFFER_CONTENTS_VERTICES:
		GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle))
		break;
	case LAG_GPU_BUFFER_CONTENTS_INDICES:
		//GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY, handle));
		break;
	case LAG_GPU_BUFFER_CONTENTS_UNIFORMS:
		break;
	case LAG_GPU_BUFFER_CONTENTS_OTHER:
		break;
	default:
		break;
	}
}

GLbitfield GL4GpuBuffer::convertFlagsToGL(uint32 flags)
{
	GLbitfield res = 0;
	if (checkForFlag(LAG_GPU_BUFFER_USAGE_DYNAMIC)) res |= GL_DYNAMIC_STORAGE_BIT;
	if (checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_READ)) res |= GL_MAP_READ_BIT;
	if (checkForFlag(LAG_GPU_BUFFER_USAGE_MAP_WRITE)) res |= GL_MAP_WRITE_BIT;
	return res;
}