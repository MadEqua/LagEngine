#include "VertexDescription.h"

using namespace Lag;

VertexAttribute::VertexAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length,
	VertexAttributeType type, uint8 index, bool isNormalized) :
	semantic(semantic),
	offset(offset),
	length(length),
	type(type),
	index(index),
	isNormalized(isNormalized)
{
}

VertexAttribute::~VertexAttribute()
{
}

uint16 VertexAttribute::getByteSize() const
{
	return getTypeByteSize(type) * length;
}

uint8 VertexAttribute::getTypeByteSize(VertexAttributeType type)
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

///////////////////////////////////////
///////////////////////////////////////

VertexDescription::VertexDescription() 
{
}

VertexDescription::~VertexDescription()
{
}

void VertexDescription::addAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length, VertexAttributeType type, uint8 index, bool isNormalized)
{
	VertexAttribute attr(semantic, offset, length, type, index, isNormalized);
	attributes.push_back(attr);
}