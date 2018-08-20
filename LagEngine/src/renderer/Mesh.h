#pragma once

#include "XmlResource.h"
#include "SubMesh.h"
#include "Types.h"

#include <memory>
#include <vector>
#include <string>

namespace Lag {
    class SubMesh;
    class VertexDescription;
    class MemoryBuffer;

    /*
    * Can be loaded using the Assimp library or manually created.
    * It depends on the SubMesh class which contains all the actual data. A mesh has at least one SubMesh.
    * Usually associated with any number of Entities.
    *
    * TODO: Support animations.
    */
    class Mesh : public XmlResource {
    public:
        inline const std::vector<std::unique_ptr<SubMesh>> &getSubMeshes() const { return subMeshes; }

        /*
         * Use lock to edit the vertices and indices. Unlock to finalize.
         * Lock will reset all the current submeshes.
         */
        void lock();
        void unlock();

        void setVertices(const MemoryBuffer &vertices, uint32 vertexCount, const VertexDescription &vertexDescription);
        void setVertices(const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription);

        template <typename T> void setIndices(const std::vector<T> &indices);
        void setIndices(const byte *indices, uint32 indexCount);

        inline bool hasData() const { return !subMeshes.empty(); }

    private:
        friend class MeshBuilder;

        Mesh();
        explicit Mesh(const std::string &file);

        bool loadImplementation() override;
        void unloadImplementation() override;

        void setVertices(const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription, uint32 subMeshIndex);
        void setIndices(const byte *indices, uint32 indexCount, uint32 subMeshIndex);

        std::vector<std::unique_ptr<SubMesh>> subMeshes;

        bool isLocked;
    };
}