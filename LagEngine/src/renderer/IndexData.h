#pragma once

#include "../Types.h"

namespace Lag
{
	class IndexBuffer;

	enum IndexType
	{
		UINT8,
		UINT16,
		UINT32
	};

	class IndexDescription
	{
	public:
		IndexDescription(IndexType type);
		~IndexDescription();
	
		static uint8 getTypeByteSize(IndexType type);

	private:
		IndexType type;
	};

	/*
	* Collects together all the index-related information used to render geometry.
	* Allows the same IndexBuffer to contain many different objects data (and/or formats) at different points.
	*/
	class IndexData
	{
	public:
		IndexData(IndexType type);
		~IndexData();

		//How to interpret the indexes
		IndexDescription indexDescription;

		//Where to get the index from
		IndexBuffer *indexBuffer;

		//Offset from the start of the buffer
		uint32 indexStart;

		//How many indices
		uint32 indexCount;
	};
}