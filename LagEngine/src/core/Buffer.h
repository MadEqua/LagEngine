#pragma once

#include "../Types.h"

namespace Lag
{
	/*
	* Typical interface for a generic fixed-size region of memory.
	*/
	class Buffer
	{
	public:
		explicit Buffer(uint32 sizeBytes);
		virtual ~Buffer() = default;

		//Lock a buffer to enable reads/writes. Unlock after changes are done.
		virtual void lock(uint32 offset, uint32 length);
		virtual void lock();
		virtual void unlock();

		//Get a direct pointer to the locked buffer data
		virtual byte* map();
		virtual void unmap();

		virtual void write(uint32 offset, uint32 length, byte* src);
		virtual void read(uint32 offset, uint32 length, byte* dst);

		inline uint32 getSize() const { return sizeBytes; }

	protected:
		virtual void lockImplementation(uint32 offset, uint32 length) = 0;
		virtual void unlockImplementation() = 0;

		virtual byte* mapImplementation() = 0;
		virtual void unmapImplementation() = 0;

		virtual void writeImplementation(uint32 offset, uint32 length, byte* src) = 0;
		virtual void readImplementation(uint32 offset, uint32 length, byte* dst) = 0;

		bool isMapped;
		bool isLocked;
		uint32 offsetLocked, lengthLocked;
		uint32 sizeBytes;
	};
}