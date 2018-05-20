#pragma once

#include "GL4GpuBuffer.h"
#include "../../resources/GpuBufferManager.h"

namespace Lag
{
	class GpuBuffer;

	class GL4GpuBufferBuilder : public GpuBufferBuilder
	{
	public:
		virtual GL4GpuBuffer* build(const uint32 &name) const override
		{
			return new GL4GpuBuffer(itemCount * itemSizeBytes, flags, contents, useMirrorBuffer);
		}
	};
}