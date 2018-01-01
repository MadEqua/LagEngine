#include "IndexData.h"

using namespace Lag;

uint8 IndexData::getTypeByteSize() const
{
	switch (indexType)
	{
	case LAG_IDX_TYPE_UINT8:
		return 1;
	case LAG_IDX_TYPE_UINT16:
		return 2;
	case LAG_IDX_TYPE_UINT32:
		return 4;
	default:
		return 0;
	}
}