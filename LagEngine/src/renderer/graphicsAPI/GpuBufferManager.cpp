#include "GpuBufferManager.h"

#include "../VertexDescription.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager() 
{
}

GpuBufferManager::~GpuBufferManager()
{
	for (auto vb : vertexBuffers)
		delete vb;

	for (auto vd : vertexDescriptions)
		delete vd;
}

VertexDescription& GpuBufferManager::createVertexDescription()
{
	VertexDescription *vd = new VertexDescription();
	vertexDescriptions.push_back(vd);
	return *vd;
}