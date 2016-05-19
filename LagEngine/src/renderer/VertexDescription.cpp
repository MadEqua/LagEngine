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
	case LAG_VX_ATTR_TYPE_INT8:
	case LAG_VX_ATTR_TYPE_UINT8:
		return 1;
	case LAG_VX_ATTR_TYPE_HALF_FLOAT:
	case LAG_VX_ATTR_TYPE_FIXED:
	case LAG_VX_ATTR_TYPE_INT16:
	case LAG_VX_ATTR_TYPE_UINT16:
		return 2;
	case LAG_VX_ATTR_TYPE_FLOAT:
	case LAG_VX_ATTR_TYPE_INT32:
	case LAG_VX_ATTR_TYPE_UINT32:
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