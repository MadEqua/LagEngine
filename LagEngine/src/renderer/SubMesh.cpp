#include "SubMesh.h"

#include "VertexData.h"
#include "IndexData.h"

using namespace Lag;

SubMesh::SubMesh(VertexData &vxData, IndexData &idxData) :
	vertexData(vxData), indexData(&idxData)
{
}

SubMesh::SubMesh(VertexData &vxData) :
	vertexData(vxData), indexData(nullptr)
{
}

SubMesh::~SubMesh()
{
	delete &vertexData;

	if (indexData != nullptr)
		delete indexData;
}