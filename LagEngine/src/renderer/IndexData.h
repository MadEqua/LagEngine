#pragma once

#include "../Types.h"

namespace Lag
{
	class GpuBuffer;

	enum IndexType
	{
		LAG_IDX_TYPE_UINT8,
		LAG_IDX_TYPE_UINT16,
		LAG_IDX_TYPE_UINT32
	};

	/*class IndexDescription
	{
	public:
		IndexDescription(IndexType type);
		~IndexDescription();

	private:
		IndexType type;
	};*/

	/*
	* Collects together all the index-related information used to render geometry.
	* Allows the same IndexBuffer to contain many different objects data (and/or formats) at different points.
	*/
	class IndexData
	{
	public:
		IndexData();
		~IndexData();

		//How to interpret the indexes
		IndexType indexType;

		//Where to get the index from
		GpuBuffer *indexBuffer;

		//Offset from the start of the buffer (bytes)
		uint32 indexStart;

		//How many indices
		uint32 indexCount;

		static uint8 getTypeByteSize(IndexType type);
	};
}