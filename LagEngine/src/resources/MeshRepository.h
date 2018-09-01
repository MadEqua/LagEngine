#pragma once

#include "Handle.h"

namespace Lag {

    class MeshManager;
    class Mesh;
    
    /*
    * Knows how to create common meshes that may be procedurally generated. Cube, Sphere, Plane, etc.
    */
    class MeshRepository {

        friend class MeshManager;

    protected:
        explicit MeshRepository(MeshManager &meshManager);

        Handle<Mesh> getCube() const;
        Handle<Mesh> getCubeInsides() const;
        Handle<Mesh> getAxisGizmo() const; //To render as Lines
        Handle<Mesh> getPlaneXZ() const;

    private:
        MeshManager &meshManager;
    };
};