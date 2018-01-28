#include "GpuBufferManager.h"

using namespace Lag;

GpuBufferBuilder::GpuBufferBuilder() :
	contents(LAG_GPU_BUFFER_CONTENTS_VERTICES),
	itemCount(1),
	itemSizeBytes(1),
	flags(0),
	useMirrorBuffer(false)
{
}

GpuBufferManager::GpuBufferManager(GpuBufferBuilder* builder) :
	Manager("GpuBufferManager", builder),
	nextName(0)
{
}

GpuBuffer* GpuBufferManager::get()
{
	return Manager::get(getNextName());
}

GpuBuffer* GpuBufferManager::get(ManagedObject &parent)
{
	return Manager::get(getNextName(), parent);
}

uint32 GpuBufferManager::getNextName()
{
	return nextName++;
}