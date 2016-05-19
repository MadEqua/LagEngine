#pragma once

#include "../Types.h"

namespace Lag
{
	class VertexDescription;
	class VertexBuffer;
	
	/*
	* Collects together all the vertex-related information used to render geometry.
	* Allows the same VertexBuffer to contain many different objects data (and/or formats) at different points.
	*/
	class VertexData
	{
	public:
		VertexData();
		~VertexData();

		//How to interpret the vertices
		VertexDescription *vertexDescription;

		//Where to get the vertices from
		//TODO: maybe add a Bindings class (id -> buffer) to allow getting attributes from different buffers. (add a binding id on VertexAttribute also)
		VertexBuffer *vertexBuffer;

		//Offset from the start of the buffer
		uint32 vertexStart;

		//How many vertices
		uint32 vertexCount;
	};
}