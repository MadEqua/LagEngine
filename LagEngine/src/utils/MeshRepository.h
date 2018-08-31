#pragma once

#include "Handle.h"
#include "Mesh.h"

namespace Lag {

    class MeshManager;
    
    /*
    * Knows how to create common meshes that may be procedurally generated. Cube, Sphere, Plane, etc.
    */
    class MeshRepository {
    public:
        explicit MeshRepository(MeshManager &meshManager);

        Handle<Mesh> getCube() const;
        Handle<Mesh> getAxisGizmo() const; //To render as Lines
        Handle<Mesh> getPlaneXZ() const;

    private:
        void setMeshVerticesAndIndices(Handle<Mesh> handle, const float *vertices, const uint8 *indices, uint32 vxCount, uint32 idxCount) const;
        MeshManager &meshManager;
    };
};