#pragma once

#include "../../core/Buffer.h"

namespace Lag
{
	class MemoryBuffer;
	
	enum GpuBufferUsage
	{
		LAG_GPU_BUFFER_USAGE_DYNAMIC = 1, //Set this to be able to write to the buffer data at any time
		LAG_GPU_BUFFER_USAGE_MAP_READ = 2, //Set this to be able to map the buffer
		LAG_GPU_BUFFER_USAGE_MAP_WRITE = 4
	};

	enum GpuBufferContents
	{
		LAG_GPU_BUFFER_CONTENTS_VERTICES,
		LAG_GPU_BUFFER_CONTENTS_INDICES,
		LAG_GPU_BUFFER_CONTENTS_UNIFORMS,
		LAG_GPU_BUFFER_CONTENTS_OTHER
	};
	
	/*
	* Representing a Buffer on the GPU. It may contain any kind of data.
	* The most derived implementation will most likely be GraphicsAPI dependent.
	*
	* Maintains an optional MemoryBuffer to serve as mirror for the GPU memory, making reads more efficient.
	*
	* Created and managed by GpuBufferManager.
	*/
	class GpuBuffer : public Buffer
	{
	public:
		GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror);
		GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror);
		virtual ~GpuBuffer();

		virtual void lock(uint32 offset, uint32 length) override;
		virtual void lock();
		virtual void unlock() override;

		virtual byte* map() override;
		virtual void unmap() override;

		virtual void write(uint32 offset, uint32 length, byte* src) override;
		virtual void read(uint32 offset, uint32 length, byte* dst) override;

		virtual void bind() const = 0;

		bool checkForFlag(GpuBufferUsage flagToCheck);

	protected:
		
		//Basic implementation provided. Better ones can be made on concrete classes
		virtual void updateFromMirror();

		//if useMirror = true
		MemoryBuffer *mirrorBuffer;
		bool useMirror;

		uint32 flags;
		GpuBufferContents contents;
	};
}