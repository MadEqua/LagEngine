#pragma once

#include <string>
#include <vector>

namespace Lag
{
	class VertexData;
	class IndexData;
	class Texture;
	
	/*
	* A SubMesh contains all the geometry related data needed for rendering (encapsulated on VertexData and IndexData).
	* It belongs to a parent Mesh, and should be initialized by it.
	*/
	class SubMesh
	{
	private:
		friend class Mesh;
		SubMesh(VertexData &vxData, IndexData &idxData, const std::vector<Texture*> &textures);
		SubMesh(VertexData &vxData, const std::vector<Texture*> &textures);
		~SubMesh();

		VertexData *vertexData;
		IndexData *indexData; //optional
		std::vector<Texture*> textures;
	};
}