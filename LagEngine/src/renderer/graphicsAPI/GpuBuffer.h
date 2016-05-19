#pragma once

#include "../../core/Buffer.h"

namespace Lag
{
	class MemoryBuffer;
	
	/*
	* Representing a Buffer on the GPU. It may contain any kind of data.
	* The most derived implementation will most likely be GraphicsAPI dependent.
	*
	* Maintains an optional MemoryBuffer to serve as mirror for the GPU memory, making reads more efficient.
	*
	* Created and managed by GpuBufferManager.
	* TODO: usage types: static/dynamic read/write, options for changing data and resend to gpu
	*/
	class GpuBuffer : public Buffer
	{
	public:
		GpuBuffer(uint32 sizeBytes, bool useMirror);
		virtual ~GpuBuffer();

		virtual void lock(uint32 offset, uint32 length) override;
		virtual void lock();
		virtual void unlock() override;

		virtual byte* map() override;
		virtual void unmap() override;

		virtual void write(uint32 offset, uint32 length, byte* src) override;
		virtual void read(uint32 offset, uint32 length, byte* dst) override;

	protected:
		
		//Basic implementation provided. Better ones can be made on concrete classes
		virtual void updateFromMirror();

		//if useMirror = true
		MemoryBuffer *mirrorBuffer;
		bool useMirror;
	};
}