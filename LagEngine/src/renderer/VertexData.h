#pragma once

#include "../Types.h"

namespace Lag
{
	class InputDescription;
	
	/*
	* Collects together the vertex-related information used to render geometry.
	* Allows the same VertexBuffer to contain many different objects data (and/or formats) at different points.
	*/
	class VertexData
	{
	public:
		//InputDescription object that describes the vertex data. (Format and source)
		//Used to feed the pipeline entry-point.
		//May (and should) be reutilized between many VertexDatas
		InputDescription *inputDescription;

		//Offset from the start of the buffer (bytes)
		uint32 vertexStart;

		//How many vertices
		uint32 vertexCount;
	};
}