#pragma once

#include "../resources/Resource.h"

namespace Lag
{
	class VertexData;
	class IndexData;
	
	/*
	* TODO: Support animations.
	*/
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual bool load() override;
		virtual void unload() override;

	private:
		VertexData *vertexData;
		IndexData *indexData;
	};
}