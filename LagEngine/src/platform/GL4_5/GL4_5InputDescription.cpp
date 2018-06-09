#include "GL4_5InputDescription.h"
#include "GL4_5GpuBuffer.h"

using namespace Lag;

GL4InputDescription::GL4InputDescription(const VertexDescription &vertexDescription,
                                         const Handle<GpuBuffer> vertexBuffer) :
        InputDescription(vertexDescription, vertexBuffer) {
}

bool Lag::GL4InputDescription::loadImplementation() {
    glCreateVertexArrays(1, &handle);
    setupVertexAttributtes();
    return true;
}

void Lag::GL4InputDescription::unloadImplementation() {
    glDeleteVertexArrays(1, &handle);
}

void GL4InputDescription::setupVertexAttributtes() const {
    //Bind the VBO to VBO binding point 0 (of this VAO) and describes how to get vertex data from the VBO
    glVertexArrayVertexBuffer(handle, 0, dynamic_cast<const GL4GpuBuffer &>(*vertexBuffer).getHandle(),

            //Not sending an offset within the VBO itself.
            //Relying on draw calls to use an offset (getting it from vertexStart on VertexData)
            //This allows more VAO reutilization, because there is no need for different
            //VAOs when different offsets should be used. (eg: Submeshes of a Mesh sharing the same VBO)
                              0,
                              vertexDescription.getByteSize());

    for (uint32 i = 0; i < vertexDescription.getAttributeCount(); ++i) {
        const VertexAttribute &vxAttr = vertexDescription.getAttribute(i);

        //Associate the attribute i to VBO binding point 0
        //(All attributes from the same VBO)
        glVertexArrayAttribBinding(handle, i, 0);

        //Describes how to interpret each particular vertex attribute
        glVertexArrayAttribFormat(handle, i,
                                  static_cast<GLint>(vxAttr.getLength()),
                                  convertAttributeTypeToGL(vxAttr.getType()),
                                  vxAttr.getIsNormalized() ? GL_TRUE : GL_FALSE,
                                  vxAttr.getOffset());

        //Enable the attribute
        glEnableVertexArrayAttrib(handle, i);
    }
}

GLenum GL4InputDescription::convertAttributeTypeToGL(VertexAttributeType type) const {
    switch (type) {
        case VertexAttributeType::HALF_FLOAT:
            return GL_HALF_FLOAT;
        case VertexAttributeType::FLOAT:
            return GL_FLOAT;
        case VertexAttributeType::FIXED:
            return GL_FIXED;
        case VertexAttributeType::INT8:
            return GL_BYTE;
        case VertexAttributeType::UINT8:
            return GL_UNSIGNED_BYTE;
        case VertexAttributeType::INT16:
            return GL_SHORT;
        case VertexAttributeType::UINT16:
            return GL_UNSIGNED_SHORT;
        case VertexAttributeType::INT32:
            return GL_INT;
        case VertexAttributeType::UINT32:
            return GL_UNSIGNED_INT;
        default:
            return 0;
    }
}