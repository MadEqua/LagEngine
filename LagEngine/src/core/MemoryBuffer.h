#pragma once

#include "Buffer.h"

namespace Lag {
    /*
    * Buffer that stores its data on main memory.
    */
    class MemoryBuffer : public Buffer {
    public:
        MemoryBuffer(uint32 sizeBytes, byte *data);
        explicit MemoryBuffer(uint32 sizeBytes);
        ~MemoryBuffer() override;

        const byte *getData() const;

    protected:
        void lockImplementation(uint32 offset, uint32 length) override;
        void unlockImplementation() override;

        byte *mapImplementation() override;
        void unmapImplementation() override;

        void writeImplementation(uint32 offset, uint32 length, const byte *src) override;
        void readImplementation(uint32 offset, uint32 length, byte *dst) override;

        byte *data;
    };
}