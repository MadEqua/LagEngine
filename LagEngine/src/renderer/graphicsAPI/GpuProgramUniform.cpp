#include "GpuProgramUniform.h"

using namespace Lag;

GpuProgramUniform::GpuProgramUniform(uint32 location, GpuProgramUniformSize size,
	GpuProgramUniformType type, GpuProgramUniformSemantic semantic, 
	void* dataLocation) :
	location(location),
	size(size),
	type(type),
	dataPtr(dataLocation),
	semantic(semantic),
	isDirty(true)
{
}

GpuProgramUniform::~GpuProgramUniform()
{
}

uint8 GpuProgramUniform::getSize() const
{
	switch (size)
	{
	case LAG_GPU_PROG_UNIFORM_SIZE_1:
		return 1;
	case LAG_GPU_PROG_UNIFORM_SIZE_2:
		return 2;
	case LAG_GPU_PROG_UNIFORM_SIZE_3:
		return 3;
	case LAG_GPU_PROG_UNIFORM_SIZE_4:
		return 4;
	default:
		return 0;
	}
}

uint8 GpuProgramUniform::getSizeBytes() const
{
	uint8 size = getSize();
	
	switch (type)
	{
	case LAG_GPU_PROG_UNIFORM_TYPE_BOOL:
		return sizeof(bool) * size;
	case LAG_GPU_PROG_UNIFORM_TYPE_FLOAT:
		return sizeof(float) * size;
	case LAG_GPU_PROG_UNIFORM_TYPE_INT32:
		return sizeof(int32) * size;
	case LAG_GPU_PROG_UNIFORM_TYPE_UINT32:
		return sizeof(uint32) * size;
	//case LAG_GPU_PROG_UNIFORM_TYPE_SAMPLER:
		//return sizeof(?);
	case LAG_GPU_PROG_UNIFORM_TYPE_MATRIX:
		return sizeof(float) * size * size;
	default:
		return 0;
	}
}

void GpuProgramUniform::internalSetValue(void* value)
{
	uint8 bytes = getSizeBytes();
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