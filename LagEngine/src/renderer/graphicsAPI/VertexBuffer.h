#pragma once

#include "../../Types.h"
#include "../GpuBuffer.h"

namespace Lag
{
	/*
	* Abstract buffer used to store vertex data. Each GraphicsAPI should implement its own.
	*/
	class VertexBuffer : public GpuBuffer
	{
	public:
		VertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirror);
		virtual ~VertexBuffer();

	protected:
		uint32 vertexCount;
	};
}