#include "GpuProgramStageUniforms.h"

#include "graphicsAPI/GpuProgramUniformFactory.h"
#include "graphicsAPI/GpuProgramUniform.h"

using namespace Lag;

GpuProgramStageUniforms::GpuProgramStageUniforms(GpuProgramUniformFactory *factory) :
	Manager(factory)
{
}

GpuProgramStageUniforms::~GpuProgramStageUniforms()
{
}

bool GpuProgramStageUniforms::create(const std::string &name)
{
	GpuProgramUniformFactory *concreteFactory = static_cast<GpuProgramUniformFactory*>(factory);
	concreteFactory->dataLocation = reinterpret_cast<void*>(dataPool.data() + dataPool.size());

	GpuProgramUniform *uniform = concreteFactory->create();
	uint8 sizeBytes = uniform->getSizeBytes();
	dataPool.resize(dataPool.size() + sizeBytes);
	return add(name, uniform);
}

void GpuProgramStageUniforms::sendToGpu() const
{
	for (auto &un : objects)
		un.second->sendToGpu();
}