#pragma once

#include "XmlResource.h"
#include <vector>
#include <string>

namespace Lag {
    class SubMesh;

    /*
    * Class that loads a mesh file using the Assimp library.
    * It depends on the SubMesh class which contains all the actual data. A mesh has at least one SubMesh.
    * Usually associated with any number of Entities.
    *
    * TODO: Support animations.
    */
    class Mesh : public XmlResource {
    public:
        inline const std::vector<SubMesh *> &getSubMeshes() const { return subMeshes; }

    private:
        friend class MeshBuilder;

        explicit Mesh(const std::string &file);

        bool loadImplementation() override;
        void unloadImplementation() override;

        std::vector<SubMesh *> subMeshes;
    };
}