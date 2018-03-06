#pragma once

#include "../Types.h"
#include "../core/Handle.h"

namespace Lag
{
	class GpuBuffer;

	enum IndexType
	{
		LAG_IDX_TYPE_UINT8,
		LAG_IDX_TYPE_UINT16,
		LAG_IDX_TYPE_UINT32
	};

	/*
	* Collects together all the index-related information used to render geometry.
	* Allows the same IndexBuffer to contain many different objects data (and/or formats) at different points.
	*/
	class IndexData
	{
	public:
		//How to interpret the indices
		IndexType indexType;

		//Where to get the indices from
		Handle<GpuBuffer> indexBuffer;

		//Offset from the start of the buffer (bytes)
		uint32 indexStart;

		//How many indices
		uint32 indexCount;

		uint8 getTypeByteSize() const;
	};
}