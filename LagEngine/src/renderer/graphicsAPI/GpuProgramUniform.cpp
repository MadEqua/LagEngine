#include "GpuProgramUniform.h"

#include "../GpuProgramUniformDescription.h"

using namespace Lag;

GpuProgramUniform::GpuProgramUniform(const GpuProgramUniformDescription &description, void* dataLocation) :
	description(description),
	dataPtr(dataLocation),
	isDirty(true)
{
}

GpuProgramUniform::~GpuProgramUniform()
{
}

void GpuProgramUniform::internalSetValue(void* value)
{
	uint8 bytes = description.getSizeBytes();
	memcpy(dataPtr, value, bytes);
	isDirty = true;
}

template<class T>
void GpuProgramUniform::setValue(T value)
{
	internalSetValue(&value);
}

template<class T>
void GpuProgramUniform::setValue(T *value)
{
	internalSetValue(value);
}

void GpuProgramUniform::sendToGpu()
{
	sendToGpuImpl();
	isDirty = false;
}