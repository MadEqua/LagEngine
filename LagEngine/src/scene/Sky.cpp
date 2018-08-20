#include "Sky.h"

#include "Root.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "VertexDescription.h"

using namespace Lag;

Sky::Sky(const std::string &materialName) {
    MeshManager &meshManager = Root::getInstance().getMeshManager();

    auto mesh = meshManager.getEmpty("skyMesh");
    if(!mesh->hasData()) {
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

        mesh->lock();

        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

        mesh->setVertices(reinterpret_cast<byte *>(vertices), VERTEX_COUNT, vxDesc);
        mesh->setIndices(static_cast<byte *>(indices), INDEX_COUNT);

        mesh->unlock();
    }

    setMesh(mesh);
    setMaterial(materialName);
}