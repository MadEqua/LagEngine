#include "SubMesh.h"

#include "VertexData.h"
#include "IndexData.h"

using namespace Lag;

SubMesh::SubMesh(VertexData &vxData, IndexData &idxData, const std::vector<Texture*> &textures) :
	vertexData(vxData), indexData(&idxData),
	textures(textures)
{
}

SubMesh::SubMesh(VertexData &vxData, const std::vector<Texture*> &textures) :
	vertexData(vxData), indexData(nullptr),
	textures(textures)
{
}

SubMesh::~SubMesh()
{
	delete &vertexData;

	if (indexData != nullptr)
		delete indexData;
}