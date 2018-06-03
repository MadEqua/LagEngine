#pragma once

#include "Types.h"
#include "Buffer.h"
#include "ManagedObject.h"

namespace Lag {
    class MemoryBuffer;

    enum class GpuBufferUsage : uint8 {
        DYNAMIC = 1, //Set this to be able to write to the buffer data at any time
        MAP_READ = 2, //Set this to be able to map the buffer
        MAP_WRITE = 4
    };

    enum class GpuBufferContents : uint8 {
        VERTICES,
        INDICES,
        UNIFORMS,
        OTHER
    };

    /*
    * Representing a Buffer on the GPU. It may contain any kind of data.
    * The most derived implementation will most likely be GraphicsAPI dependent.
    *
    * Maintains an optional MemoryBuffer to serve as mirror for the GPU memory, making reads more efficient.
    *
    * Created and managed by GpuBufferManager.
    */
    class GpuBuffer : public Buffer, public ManagedObject {
    public:
        void lock(uint32 offset, uint32 length) override;
        void lock() override;
        void unlock() override;

        byte *map() override;
        void unmap() override;

        void write(uint32 offset, uint32 length, byte *src) override;
        void read(uint32 offset, uint32 length, byte *dst) override;

        bool checkForFlag(GpuBufferUsage flagToCheck);

        bool loadImplementation() override;
        void unloadImplementation() override;

        void setUseMirror(bool use);

    protected:
        //GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror);
        GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror);

        //Basic implementation provided. Better ones can be made on concrete classes
        virtual void updateFromMirror();

        //if useMirror = true
        MemoryBuffer *mirrorBuffer;
        bool useMirror;

        uint32 flags;
        GpuBufferContents contents;
    };
}