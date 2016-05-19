#include "IndexData.h"

using namespace Lag;

IndexDescription::IndexDescription(IndexType type) :
	type(type)
{
}

IndexDescription::~IndexDescription()
{
}

uint8 IndexDescription::getTypeByteSize(IndexType type)
{
	switch (type)
	{
	case UINT8:
		return 1;
	case UINT16:
		return 2;
	case UINT32:
		return 4;
	default:
		return 0;
	}
}

IndexData::IndexData(IndexType type) :
	indexDescription(type)
{
}

IndexData::~IndexData()
{
}