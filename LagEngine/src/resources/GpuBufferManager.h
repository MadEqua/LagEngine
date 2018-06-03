#pragma once

#include <vector>
#include "Manager.h"
#include "GpuBuffer.h"
#include "Types.h"

namespace Lag {
    class GpuBufferBuilder : public IManagedObjectBuilder<uint32, GpuBuffer> {
    public:
        GpuBufferBuilder();

        GpuBufferContents contents;
        int32 itemCount;
        uint32 itemSizeBytes, flags;
        bool useMirrorBuffer;
    };


    /*
    * Creates and manages all GpuBuffers.
    */
    class GpuBufferManager : public Manager<uint32, GpuBuffer> {
    public:
        explicit GpuBufferManager(GpuBufferBuilder *builder);
        uint32 getNextName();

        //Convenience methods for the most common operations
        //Do not forget to set the builder before calling.
        Handle<GpuBuffer> get();

    protected:
        uint32 nextName;
    };
}