#include "GL4InputDescription.h"
#include "../../renderer/VertexDescription.h"
#include "GL4GpuBuffer.h"

using namespace Lag;

GL4InputDescription::GL4InputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer) :
	InputDescription(vertexDescription, vertexBuffer)
{
	GL_ERROR_CHECK(glCreateVertexArrays(1, &handle))
	setupVertexAttributtes();
}

GL4InputDescription::~GL4InputDescription()
{
	GL_ERROR_CHECK(glDeleteVertexArrays(1, &handle))
}

void GL4InputDescription::setupVertexAttributtes() const
{
	//Bind the VBO to VBO binding point 0 (of this VAO) and describes how to get vertex data from the VBO
	GL_ERROR_CHECK(glVertexArrayVertexBuffer(handle, 0,
		static_cast<const GL4GpuBuffer&>(vertexBuffer).getHandle(),

		//Not sending an offset within the VBO itself. 
		//Relying on draw calls to use an offset (getting it from vertexStart on VertexData)
		//This allows more VAO reutilization, because there is no need for different
		//VAOs when different offsets should be used. (eg: Submeshes of a Mesh sharing the same VBO)
		0,
		vertexDescription.getByteSize()))

	for (uint32 i = 0; i < vertexDescription.getAttributeCount(); ++i)
	{
		const VertexAttribute &vxAttr = vertexDescription.getAttribute(i);

		//Associate the attribute i to VBO binding point 0
		//(All attributes from the same VBO)
		GL_ERROR_CHECK(glVertexArrayAttribBinding(handle, i, 0))

		//Describes how to interpret each particular vertex attribute
		GL_ERROR_CHECK(glVertexArrayAttribFormat(handle, i,
			static_cast<GLint>(vxAttr.getLength()),
			convertAttributeTypeToGL(vxAttr.getType()),
			vxAttr.getIsNormalized() ? GL_TRUE : GL_FALSE,
			vxAttr.getOffset()))

		//Enable the attribute
		GL_ERROR_CHECK(glEnableVertexArrayAttrib(handle, i))
	}
}

GLenum GL4InputDescription::convertAttributeTypeToGL(VertexAttributeType type) const
{
	switch (type)
	{
	case LAG_VX_ATTR_TYPE_HALF_FLOAT:
		return GL_HALF_FLOAT;
	case LAG_VX_ATTR_TYPE_FLOAT:
		return GL_FLOAT;
	case LAG_VX_ATTR_TYPE_FIXED:
		return GL_FIXED;
	case  LAG_VX_ATTR_TYPE_INT8:
		return GL_BYTE;
	case LAG_VX_ATTR_TYPE_UINT8:
		return GL_UNSIGNED_BYTE;
	case LAG_VX_ATTR_TYPE_INT16:
		return GL_SHORT;
	case LAG_VX_ATTR_TYPE_UINT16:
		return GL_UNSIGNED_SHORT;
	case LAG_VX_ATTR_TYPE_INT32:
		return GL_INT;
	case LAG_VX_ATTR_TYPE_UINT32:
		return GL_UNSIGNED_INT;
	default:
		return -1;
	}
}