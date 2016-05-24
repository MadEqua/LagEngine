#include "GpuBufferManager.h"

#include "VertexBuffer.h"
#include "../VertexDescription.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager() 
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"GpuBufferManager", "Initialized successfully.");
}

GpuBufferManager::~GpuBufferManager()
{
	for (auto vb : vertexBuffers)
		delete vb;

	for (auto vd : vertexDescriptions)
		delete vd;

	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"GpuBufferManager", "Initialized successfully.");
}

VertexDescription& GpuBufferManager::createVertexDescription()
{
	VertexDescription *vd = new VertexDescription();
	vertexDescriptions.push_back(vd);
	return *vd;
}