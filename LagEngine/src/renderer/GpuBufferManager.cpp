#include "GpuBufferManager.h"

#include "../graphicsAPIs/gl4/GL4VertexBuffer.h"
#include "VertexDescription.h"

#include "../graphicsAPIs/gl4/GL4IndexBuffer.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager(GraphicsApiType graphicsApiType) :
	graphicsApiType(graphicsApiType)
{
}

GpuBufferManager::~GpuBufferManager()
{
	for (auto vb : vertexBuffers)
		delete vb;

	for (auto vd : vertexDescriptions)
		delete vd;
}

VertexBuffer* GpuBufferManager::createVertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirrorBuffer)
{
	if (graphicsApiType == GraphicsApiType::OPENGL_4)
	{
		VertexBuffer *vb = new GL4VertexBuffer(vertexCount, vertexSizeBytes, useMirrorBuffer);
		vertexBuffers.push_back(vb);
		return vb;
	}
	return nullptr;
}

VertexDescription& GpuBufferManager::createVertexDescription()
{
	VertexDescription *vd = new VertexDescription();
	vertexDescriptions.push_back(vd);
	return *vd;
}

IndexBuffer* GpuBufferManager::createIndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirrorBuffer)
{
	if (graphicsApiType == GraphicsApiType::OPENGL_4)
	{
		IndexBuffer *ib = new GL4IndexBuffer(indexCount, indexSizeBytes, useMirrorBuffer);
		indexBuffers.push_back(ib);
		return ib;
	}
	return nullptr;
}