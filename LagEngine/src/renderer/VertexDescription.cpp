#include "VertexDescription.h"

#include <algorithm>

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
	uint8 sem = static_cast<uint8>(semantic);
	sortKey = (semantic << 4) | index;
}

VertexAttribute::~VertexAttribute()
{
}

uint16 VertexAttribute::getByteSize() const
{
	return getTypeByteSize() * length;
}

uint8 VertexAttribute::getTypeByteSize() const
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

bool VertexAttribute::operator==(const VertexAttribute &other) const
{
	return semantic == other.semantic &&
		offset == other.offset &&
		length == other.length &&
		type == other.type &&
		index == other.index &&
		isNormalized == other.isNormalized;
}

VertexAttribute::operator std::size_t() const
{
	return static_cast<uint32>(semantic) ^
		offset ^
		static_cast<uint32>(length) ^
		static_cast<uint32>(type) ^
		static_cast<uint32>(index) ^
		static_cast<uint32>(isNormalized);
}

bool VertexAttribute::operator<(const VertexAttribute &other) const
{
	return sortKey < other.sortKey;
}

///////////////////////////////////////
///////////////////////////////////////

VertexDescription::VertexDescription() 
{
}

VertexDescription::~VertexDescription()
{
}

void VertexDescription::addAttribute(VertexAttributeSemantic semantic, uint8 length, VertexAttributeType type, uint8 index, bool isNormalized)
{
	//Offset computation is below, after sorting
	VertexAttribute newAttr(semantic, -1, length, type, index, isNormalized);
	attributes.push_back(newAttr);
	
	std::sort(attributes.begin(), attributes.end());

	uint32 sum = 0;
	for (int i = 0; i < attributes.size(); ++i)
	{
		VertexAttribute &attr = attributes[i];
		attr.offset = sum;
		sum += attr.getByteSize();
	}
}

uint32 VertexDescription::getByteSize() const
{
	uint32 size = 0;
	for (const VertexAttribute &attr : attributes)
		size += attr.getByteSize();
	return size;
}

bool VertexDescription::operator==(const VertexDescription &other) const
{
	//Two VertexDescriptions are the same if they contain the same attributes, even in different orders.
	for (const VertexAttribute &attr : attributes)
	{
		auto it = std::find(other.attributes.begin(), other.attributes.end(), attr);
		if (it == other.attributes.end())
			return false;
	}

	return true;
}

VertexDescription::operator std::size_t() const
{
	std::size_t ret = 0;
	for (const VertexAttribute &attr : attributes)
	{
		ret ^= attr;
	}
	return ret;
}