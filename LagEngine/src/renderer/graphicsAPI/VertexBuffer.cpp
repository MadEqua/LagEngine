#include "VertexBuffer.h"
#include <iostream>

using namespace Lag;

VertexBufferDetails::VertexBufferDetails(uint32 vertexCount, bool interleavedVertices) :
	vertexCount(vertexCount),
	interleavedVertices(interleavedVertices)
{
	memset(enabledAttributes, 0, ATTRIBUTE_SEMANTIC_COUNT * sizeof(bool));
	memset(attributeIsNormalized, 0, ATTRIBUTE_SEMANTIC_COUNT * sizeof(bool));

	//Position is mandatory
	enableAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);
}

VertexBufferDetails::~VertexBufferDetails()
{

}

void VertexBufferDetails::enableAttribute(VertexAttributeSemantic semantic, uint8 length, VertexAttributeType type, bool isNormalized)
{
	enabledAttributes[semantic] = true;
	attributeLengths[semantic] = length;
	attributeTypes[semantic] = type;
	attributeIsNormalized[semantic] = isNormalized;
	computeOffsetAndStride(semantic);
}

void VertexBufferDetails::computeOffsetAndStride(VertexAttributeSemantic semantic)
{
	uint32 offset = 0;
	uint16 vertexSize = computeVertexBytesSize();
	
	for (int i = 0; i < ATTRIBUTE_SEMANTIC_COUNT; ++i)
	{
		if (enabledAttributes[i])
		{
			if (interleavedVertices)
			{
				attributeOffsets[i] = offset;
				offset += computeAttributeBytesSize(static_cast<VertexAttributeSemantic>(i));
				attributeStrides[i] = vertexSize;
			}
			else
			{
				attributeOffsets[i] = offset;
				uint8 attrSize = computeAttributeBytesSize(static_cast<VertexAttributeSemantic>(i));
				offset += attrSize * vertexCount;
				attributeStrides[i] = attrSize;
			}
		}
	}
}

uint16 VertexBufferDetails::computeVertexBytesSize() const
{
	uint16 sum = 0;
	for (int i = 0; i < ATTRIBUTE_SEMANTIC_COUNT; ++i)
	{
		if (enabledAttributes[i])
		{
			sum += computeAttributeBytesSize(static_cast<VertexAttributeSemantic>(i));
		}
	}
	return sum;
}

uint8 VertexBufferDetails::computeAttributeBytesSize(VertexAttributeSemantic semantic) const
{
	uint8 length = attributeLengths[semantic];
	uint8 size = getAttributeTypeBytesSize(attributeTypes[semantic]);
	return size * length;
}

uint8 VertexBufferDetails::getAttributeTypeBytesSize(VertexAttributeType type) const
{
	switch (type)
	{
	case INT8:
	case UINT8:
		return 1;
	case HALF_FLOAT:
	case FIXED:
	case INT16:
	case UINT16:
		return 2;
	case FLOAT:
	case INT32:
	case UINT32:
		return 4;
	default:
		return 0;
	}
}

//////////////////////////////////////////////
//////////////////////////////////////////////+

VertexBuffer::VertexBuffer(const VertexBufferDetails &details) :
	details(details)
{
	dataSize = details.vertexCount * details.computeVertexBytesSize();
	this->data = new byte[dataSize];
}

VertexBuffer::VertexBuffer(const VertexBufferDetails &details, const byte *data) :
	VertexBuffer(details)
{
	memcpy(this->data, data, dataSize);
}

VertexBuffer::~VertexBuffer()
{
	delete[] data;
}

void VertexBuffer::setVertexAttributeData(VertexAttributeSemantic attribute, uint32 vertexNumber, byte* data)
{
	uint16 offset = details.attributeOffsets[attribute];
	uint16 stride = details.attributeStrides[attribute];
	uint8 attrSize = details.computeAttributeBytesSize(attribute);

	uint32 firstByte = offset + vertexNumber * stride;
	memcpy((data + firstByte), data, attrSize);
}