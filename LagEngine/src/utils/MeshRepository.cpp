#include "MeshRepository.h"

#include "MeshManager.h"
#include "Constants.h"
#include "VertexDescription.h"

using namespace Lag;

MeshRepository::MeshRepository(MeshManager &meshManager) : 
    meshManager(meshManager) {
}

Handle<Mesh> MeshRepository::getCube() const {
    if(!meshManager.contains(MESH_CUBE)) {
        auto handle = meshManager.getEmpty(MESH_CUBE);

        const int VERTEX_COUNT = 8;
        const int INDEX_COUNT = 12 * 3;
        const float v = 1.0f;
        float vertices[] = {
                -v, v, v,
                -v, -v, v,
                v, -v, v,
                v, v, v,
                -v, v, -v,
                -v, -v, -v,
                v, -v, -v,
                v, v, -v
        };

        uint8 indices[INDEX_COUNT] = {
                2, 1, 0,
                0, 3, 2,
                4, 5, 6,
                6, 7, 4,
                6, 2, 7,
                2, 3, 7,
                4, 1, 5,
                4, 0, 1,
                5, 1, 2,
                5, 2, 6,
                4, 3, 0,
                4, 7, 3
        };
        setMeshVerticesAndIndices(handle, vertices, indices, VERTEX_COUNT, INDEX_COUNT);
        return handle;
    }

    return meshManager.get(MESH_CUBE);
}

Handle<Mesh> MeshRepository::getAxisGizmo() const {
    if(!meshManager.contains(MESH_AXIS_GIZMO)) {
        auto handle = meshManager.getEmpty(MESH_AXIS_GIZMO);

        const uint32 VERTEX_COUNT = 4;
        const uint32 INDEX_COUNT = 6;
        float vertices[] = {
                0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
        };
        uint8 indices[] = {
                0, 1,
                0, 2,
                0, 3
        };

        setMeshVerticesAndIndices(handle, vertices, indices, VERTEX_COUNT, INDEX_COUNT);
        return handle;
    }

    return meshManager.get(MESH_AXIS_GIZMO);
}

Handle<Mesh> MeshRepository::getPlaneXZ() const {
    if(!meshManager.contains(MESH_PLANE_XZ)) {
        auto handle = meshManager.getEmpty(MESH_PLANE_XZ);

        const uint32 VERTEX_COUNT = 4;
        const uint32 INDEX_COUNT = 6;
        float vertices[] = {
                0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f
        };
        uint8 indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        setMeshVerticesAndIndices(handle, vertices, indices, VERTEX_COUNT, INDEX_COUNT);
        return handle;
    }

    return meshManager.get(MESH_AXIS_GIZMO);
}

void MeshRepository::setMeshVerticesAndIndices(Handle<Mesh> handle, const float *vertices, const uint8 *indices, uint32 vxCount, uint32 idxCount) const {
    handle->lock();

    VertexDescription vxDesc;
    vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

    handle->setVertices(reinterpret_cast<const byte *>(vertices), vxCount, vxDesc);
    handle->setIndices(static_cast<const byte *>(indices), idxCount);

    handle->unlock();
}
