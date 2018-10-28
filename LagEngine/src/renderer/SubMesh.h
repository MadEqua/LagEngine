#pragma once

#include <vector>
#include <memory>

#include "Types.h"
#include "VertexData.h"
#include "IndexData.h"
#include "IBoundingVolume.h"

#include "AABB.h"

namespace Lag {
    
    class VertexDescription;
    
    /*
    * A SubMesh contains all the geometry related data needed for rendering (encapsulated on VertexData and IndexData).
    * It belongs to a parent Mesh, and should be initialized by it.
    */
    class SubMesh {
    public:
        inline VertexData const* getVertexData() const { return vertexData.get(); }
        inline IndexData const* getIndexData() const { return indexData.get(); }

        inline const IBoundingVolume& getBoundingVolume() const { return *boundingVolume; }

    private:
        friend class Mesh;

        SubMesh() = default;
        SubMesh(VertexData &vxData, IndexData &idxData);
        explicit SubMesh(VertexData &vxData);

        void initBoundingVolume(const IBoundingVolume &parentBoundingVolume, const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription);

        std::unique_ptr<VertexData> vertexData;
        std::unique_ptr<IndexData> indexData; //optional
        std::unique_ptr<IBoundingVolume> boundingVolume;
    };
}