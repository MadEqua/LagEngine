#include "GpuBufferManager.h"

#include "GpuBuffer.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuBufferManager::GpuBufferManager() 
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GpuBufferManager", "Initialized successfully.");
}

GpuBufferManager::~GpuBufferManager()
{
	for (auto vb : vertexBuffers)
		delete vb;

	for (auto ib : indexBuffers)
		delete ib;

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"GpuBufferManager", "Destroyed successfully.");
}