#include "GpuBufferManager.h"

#include "VertexDescription.h"

#include "graphicsAPI/VertexBuffer.h"
#include "graphicsAPI/IndexBuffer.h"
#include "graphicsAPI/VertexBufferFactory.h"
#include "graphicsAPI/IndexBufferFactory.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager(VertexBufferFactory *vxBufferFactory, IndexBufferFactory *idxBufferFactory) :
	vertexBufferFactory(vxBufferFactory),
	indexBufferFactory(idxBufferFactory)
{
}

GpuBufferManager::~GpuBufferManager()
{
	for (auto vb : vertexBuffers)
		delete vb;

	for (auto vd : vertexDescriptions)
		delete vd;

	delete indexBufferFactory;
	delete vertexBufferFactory;
}

VertexBuffer* GpuBufferManager::createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer)
{
	vertexBufferFactory->vertexCount = vertexCount;
	vertexBufferFactory->vertexSizeBytes = vertexSizeBytes;
	vertexBufferFactory->useMirrorBuffer = useMirrorBuffer;

	VertexBuffer *vb = vertexBufferFactory->create();
	vertexBuffers.push_back(vb);
	return vb;
}

VertexDescription& GpuBufferManager::createVertexDescription()
{
	VertexDescription *vd = new VertexDescription();
	vertexDescriptions.push_back(vd);
	return *vd;
}

IndexBuffer* GpuBufferManager::createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer)
{
	indexBufferFactory->indexCount = indexCount;
	indexBufferFactory->indexSizeBytes = indexSizeBytes;
	indexBufferFactory->useMirrorBuffer = useMirrorBuffer;

	IndexBuffer *ib = indexBufferFactory->create();
	indexBuffers.push_back(ib);
	return ib;
}