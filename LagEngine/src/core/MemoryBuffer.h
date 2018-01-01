#pragma once

#include "Buffer.h"

namespace Lag
{
	/*
	* Buffer that stores its data on main memory.
	*/
	class MemoryBuffer : public Buffer
	{
	public:
		MemoryBuffer(uint32 sizeBytes, byte* data);
		explicit MemoryBuffer(uint32 sizeBytes);
		virtual ~MemoryBuffer() override;

	protected:
		virtual void lockImplementation(uint32 offset, uint32 length) override;
		virtual void unlockImplementation() override;

		virtual byte* mapImplementation() override;
		virtual void unmapImplementation() override;

		virtual void writeImplementation(uint32 offset, uint32 length, byte* src) override;
		virtual void readImplementation(uint32 offset, uint32 length, byte* dst) override;

		byte *data;
	};
}