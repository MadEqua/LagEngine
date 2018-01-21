#pragma once

#include "GL4Includes.h"
#include "../../renderer/graphicsAPI/GpuBuffer.h"

namespace Lag
{
	class GL4GpuBuffer : public GpuBuffer
	{
	public:
		virtual void lockImplementation(uint32 offset, uint32 length) override;
		virtual void unlockImplementation() override;

		virtual byte* mapImplementation() override;
		virtual void unmapImplementation() override;

		virtual void writeImplementation(uint32 offset, uint32 length, byte* src) override;
		virtual void readImplementation(uint32 offset, uint32 length, byte* dst) override;

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		inline GLuint getHandle() const { return handle; }

	private:
		friend class GL4GpuBufferBuilder;
		//GL4GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror);
		GL4GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror);

		GLuint handle;
		GLbitfield convertFlagsToGL(uint32 flags);
	};
}