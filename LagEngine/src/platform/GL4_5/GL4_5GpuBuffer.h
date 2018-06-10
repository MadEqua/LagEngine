#pragma once

#include "GL/glew.h"
#include "GpuBuffer.h"

namespace Lag {
    class GL4_5GpuBuffer : public GpuBuffer {
    public:
        void lockImplementation(uint32 offset, uint32 length) override;
        void unlockImplementation() override;

        byte *mapImplementation() override;
        void unmapImplementation() override;

        void writeImplementation(uint32 offset, uint32 length, byte *src) override;
        void readImplementation(uint32 offset, uint32 length, byte *dst) override;

        bool loadImplementation() override;
        void unloadImplementation() override;

        inline GLuint getHandle() const { return handle; }

    private:
        friend class GL4_5GpuBufferBuilder;

        //GL4_5GpuBuffer(uint32 sizeBytes, byte* data, uint32 flags, GpuBufferContents contents, bool useMirror);
        GL4_5GpuBuffer(uint32 sizeBytes, uint32 flags, GpuBufferContents contents, bool useMirror);

        GLuint handle;
        GLbitfield convertFlagsToGL(uint32 flags);
    };
}