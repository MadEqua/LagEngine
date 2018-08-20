#include "SubMesh.h"

using namespace Lag;

SubMesh::SubMesh(VertexData &vxData, IndexData &idxData) :
        vertexData(&vxData), indexData(&idxData) {
}

SubMesh::SubMesh(VertexData &vxData) :
        vertexData(&vxData) {
}

const VertexData *SubMesh::getVertexData() const {
    if(vertexData) return vertexData.get();
    else return nullptr;
}

const IndexData *SubMesh::getIndexData() const {
    if(indexData) return indexData.get();
    else return nullptr;
}
