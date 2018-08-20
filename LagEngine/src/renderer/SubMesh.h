#pragma once

#include <vector>
#include <memory>

#include "VertexData.h"
#include "IndexData.h"

namespace Lag {
    /*
    * A SubMesh contains all the geometry related data needed for rendering (encapsulated on VertexData and IndexData).
    * It belongs to a parent Mesh, and should be initialized by it.
    */
    class SubMesh {
    public:
        const VertexData* getVertexData() const;
        const IndexData* getIndexData() const;

    private:
        friend class Mesh;

        SubMesh() = default;
        SubMesh(VertexData &vxData, IndexData &idxData);
        explicit SubMesh(VertexData &vxData);

        std::unique_ptr<VertexData> vertexData;
        std::unique_ptr<IndexData> indexData; //optional
    };
}