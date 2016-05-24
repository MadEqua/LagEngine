#pragma once

#include "../GpuBuffer.h"

namespace Lag
{
	/*
	* Abstract buffer used to store index data. Each GraphicsAPI should implement its own.
	*/
	class IndexBuffer : public GpuBuffer
	{
	public:
		IndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirror);
		virtual ~IndexBuffer();


	
	protected:
		uint32 indexCount;
	};
}