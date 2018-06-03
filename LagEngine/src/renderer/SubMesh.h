#pragma once

#include <vector>

namespace Lag {
    class VertexData;
    class IndexData;
    class Texture;

    /*
    * A SubMesh contains all the geometry related data needed for rendering (encapsulated on VertexData and IndexData).
    * It belongs to a parent Mesh, and should be initialized by it.
    */
    class SubMesh {
    public:
        inline const VertexData &getVertexData() const { return vertexData; }
        inline const IndexData &getIndexData() const { return *indexData; }

    private:
        friend class Mesh;

        SubMesh(VertexData &vxData, IndexData &idxData);
        explicit SubMesh(VertexData &vxData);
        ~SubMesh();

        VertexData &vertexData;
        IndexData *indexData; //optional
        //std::vector<Texture*> textures;
    };
}