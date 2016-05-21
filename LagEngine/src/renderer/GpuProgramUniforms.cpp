#include "GpuProgramUniforms.h"

#include "graphicsAPI/GpuProgramStage.h"
#include "graphicsAPI/GpuProgramUniform.h"
#include "graphicsAPI/GpuProgram.h"
#include "GpuProgramUniformDescription.h"

using namespace Lag;

GpuProgramUniforms::GpuProgramUniforms(const GpuProgram &program, 
	const std::vector<GpuProgramStage*> stages) :
	program(program)
{
	uint32 byteSize = 0;
	byte *location = dataPool.data();
	for (GpuProgramStage *stage : stages)
	{
		auto &descriptions = stage->getUniformDescriptions();
		for (const GpuProgramUniformDescription &desc : descriptions)
		{
			GpuProgramUniform *un = program.createUniform(desc, location);
			uint8 unSize = desc.getSizeBytes();
			location += unSize;
			byteSize += unSize;
		}
	}
	dataPool.reserve(byteSize);
}

GpuProgramUniforms::~GpuProgramUniforms()
{
	for (auto un : uniforms)
		delete un;
}

/*bool GpuProgramUniforms::add(GpuProgramUniform *uniform)
{
	uniforms.push_back(uniform);
}

bool GpuProgramUniforms::create(const std::string &name)
{
	GpuProgramUniformFactory *concreteFactory = static_cast<GpuProgramUniformFactory*>(factory);
	concreteFactory->dataLocation = reinterpret_cast<void*>(dataPool.data() + dataPool.size());

	GpuProgramUniform *uniform = concreteFactory->create();
	uint8 sizeBytes = uniform->getGpuProgramUniformDescription().getSizeBytes();
	dataPool.resize(dataPool.size() + sizeBytes);
	return add(name, uniform);
}*/

void GpuProgramUniforms::sendToGpu() const
{
	for (auto un : uniforms)
		un->sendToGpu();
}