#include "GpuBufferManager.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager() :
	Manager("GpuBufferManager"),
	nextName(0)
{
}

GpuBuffer* GpuBufferManager::createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, uint32 flags, bool useMirrorBuffer)
{
	GpuBuffer *vb = internalCreateVertexBuffer(vertexCount, vertexSizeBytes, flags, useMirrorBuffer);
	uint32 name = getNextName();
	if (add(name, vb) && load(name))
		return vb;
	else
		return nullptr;
}

/*GpuBuffer* GpuBufferManager::createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer)
{
	GpuBuffer *vb = internalCreateVertexBuffer(vertexCount, vertexSizeBytes, data, flags, useMirrorBuffer);
	if (addAndLoad(getNextName(), vb))
		return vb;
	else
		return nullptr;
}*/

GpuBuffer* GpuBufferManager::createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, uint32 flags, bool useMirrorBuffer)
{
	GpuBuffer *ib = internalCreateIndexBuffer(indexCount, indexSizeBytes, flags, useMirrorBuffer);
	uint32 name = getNextName();
	if (add(name, ib) && load(name))
		return ib;
	else
		return nullptr;
}

/*GpuBuffer* GpuBufferManager::createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, byte* data, uint32 flags, bool useMirrorBuffer)
{
	GpuBuffer *ib = internalCreateIndexBuffer(indexCount, indexSizeBytes, data, flags, useMirrorBuffer);
	if (addAndLoad(getNextName(), ib))
		return ib;
	else
		return nullptr;
}*/

uint32 GpuBufferManager::getNextName()
{
	return nextName++;
}