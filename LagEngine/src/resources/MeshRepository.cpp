#include "MeshRepository.h"

#include "MeshManager.h"
#include "Constants.h"
#include "VertexDescription.h"
#include "MemoryBuffer.h"

using namespace Lag;

MeshRepository::MeshRepository(MeshManager &meshManager) : 
    meshManager(meshManager) {
}

//TODO: add normals and texCoords
Handle<Mesh> MeshRepository::getCube() const {
    if(!meshManager.contains(MESH_CUBE)) {
        auto handle = meshManager.getEmpty(MESH_CUBE);

        const uint32 VERTEX_COUNT = 8;
        const uint32 INDEX_COUNT = 12 * 3;
        
        //-64 and 64 will be normalized to -0.5 and 0.5 floats
        const int8 v = 64;
        int8 vertices[] = {
                -v, v, v,
                -v, -v, v,
                v, -v, v,
                v, v, v,
                -v, v, -v,
                -v, -v, -v,
                v, -v, -v,
                v, v, -v
        };

        uint8 indicesCcw[INDEX_COUNT] = {
                0, 1, 2,
                2, 3, 0,
                6, 5, 4,
                4, 7, 6,
                7, 2, 6,
                7, 3, 2,
                5, 1, 4,
                1, 0, 4,
                2, 1, 5,
                6, 2, 5,
                0, 3, 4,
                3, 7, 4
        };

        handle->lock();
        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::INT8, 0, true);

        handle->setVertices(reinterpret_cast<const byte *>(vertices), VERTEX_COUNT, vxDesc);
        handle->setIndices(static_cast<const byte *>(indicesCcw), INDEX_COUNT);

        handle->unlock();
        return handle;
    }

    return meshManager.get(MESH_CUBE);
}

Handle<Mesh> MeshRepository::getCubeInsides() const {
    if(!meshManager.contains(MESH_CUBE_INSIDES)) {
        auto handle = meshManager.getEmpty(MESH_CUBE_INSIDES);

        const int VERTEX_COUNT = 8;
        const int INDEX_COUNT = 12 * 3;
       
        //-64 and 64 will be normalized to -0.5 and 0.5 floats
        const int8 v = 64;
        int8 vertices[] = {
                -v, v, v,
                -v, -v, v,
                v, -v, v,
                v, v, v,
                -v, v, -v,
                -v, -v, -v,
                v, -v, -v,
                v, v, -v
        };

        uint8 indicesCw[INDEX_COUNT] = {
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

        handle->lock();
        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::INT8, 0, true);

        handle->setVertices(reinterpret_cast<const byte *>(vertices), VERTEX_COUNT, vxDesc);
        handle->setIndices(static_cast<const byte *>(indicesCw), INDEX_COUNT);

        handle->unlock();
        return handle;
    }

    return meshManager.get(MESH_CUBE);
}

Handle<Mesh> MeshRepository::getAxisGizmo() const {
    if(!meshManager.contains(MESH_AXIS_GIZMO)) {
        auto handle = meshManager.getEmpty(MESH_AXIS_GIZMO);

        const uint32 VERTEX_COUNT = 4;
        const uint32 INDEX_COUNT = 6;
        uint8 vertices[] = {
                0, 0, 0,
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
        };
        uint8 indices[] = {
                0, 1,
                0, 2,
                0, 3
        };

        handle->lock();
        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::UINT8);

        handle->setVertices(reinterpret_cast<const byte *>(vertices), VERTEX_COUNT, vxDesc);
        handle->setIndices(static_cast<const byte *>(indices), INDEX_COUNT);

        handle->unlock();
        return handle;
    }

    return meshManager.get(MESH_AXIS_GIZMO);
}

Handle<Mesh> MeshRepository::getPlaneXZ() const {
    if(!meshManager.contains(MESH_PLANE_XZ)) {
        auto handle = meshManager.getEmpty(MESH_PLANE_XZ);

        const uint32 VERTEX_COUNT = 4;
        const uint32 INDEX_COUNT = 6;

        //-64 and 64 will be normalized to -0.5 and 0.5 floats
        const int8 v = 64;
        int8 positions[] = {
            -v, 0, v,
            v, 0, v,
            v, 0, -v,
            -v, 0, -v
        };

        uint8 normals[] = {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
        };

        uint8 tangents[] = {
            1, 0, 1,
            1, 0, 1,
            1, 0, 1,
            1, 0, 1,
        };

        uint8 texCoords[] = {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        };

        uint8 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::INT8, 0, true);
        vxDesc.addAttribute(VertexAttributeSemantic::NORMAL, 3, VertexAttributeType::UINT8);
        vxDesc.addAttribute(VertexAttributeSemantic::TANGENT, 3, VertexAttributeType::UINT8);
        vxDesc.addAttribute(VertexAttributeSemantic::TEXCOORD, 2, VertexAttributeType::UINT8);

        MemoryBuffer vertices(VERTEX_COUNT * vxDesc.getVertexByteSize());
        vertices.lock();

        uint32 posSize = vxDesc.getAttribute(VertexAttributeSemantic::POSITION)->getByteSize();
        uint32 normalSize = vxDesc.getAttribute(VertexAttributeSemantic::NORMAL)->getByteSize();
        uint32 tangentSize = vxDesc.getAttribute(VertexAttributeSemantic::TANGENT)->getByteSize();
        uint32 texCoordSize = vxDesc.getAttribute(VertexAttributeSemantic::TEXCOORD)->getByteSize();
        
        uint32 vxBufferOffset = 0;
        for(uint32 vx = 0; vx < VERTEX_COUNT; ++vx) {

            vertices.write(vxBufferOffset, posSize, reinterpret_cast<byte*>(&positions[vx * 3]));
            vxBufferOffset += posSize;

            vertices.write(vxBufferOffset, normalSize, reinterpret_cast<byte*>(&normals[vx * 3]));
            vxBufferOffset += normalSize;

            vertices.write(vxBufferOffset, tangentSize, reinterpret_cast<byte*>(&tangents[vx * 3]));
            vxBufferOffset += tangentSize;

            vertices.write(vxBufferOffset, texCoordSize, reinterpret_cast<byte*>(&texCoords[vx * 2]));
            vxBufferOffset += texCoordSize;
        }

        vertices.unlock();

        handle->lock();
        handle->setVertices(vertices.getData(), VERTEX_COUNT, vxDesc);
        handle->setIndices(static_cast<const byte *>(indices), INDEX_COUNT);
        handle->unlock();

        return handle;
    }

    return meshManager.get(MESH_AXIS_GIZMO);
}