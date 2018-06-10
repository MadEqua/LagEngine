#pragma once

#include "InputDescription.h"
#include "GL/glew.h"

namespace Lag {
    class GL4_5InputDescription : public InputDescription {
    public:
        bool loadImplementation() override;
        void unloadImplementation() override;

        inline GLuint getHandle() const { return handle; }

    private:
        friend class GL4_5InputDescriptionBuilder;

        GL4_5InputDescription(const VertexDescription &vertexDescription, const Handle<GpuBuffer> vertexBuffer);

        void setupVertexAttributtes() const;
        GLenum convertAttributeTypeToGL(VertexAttributeType type) const;

        GLuint handle;
    };
}