#include "AxisGizmo.h"

#include "Root.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "VertexDescription.h"

using namespace Lag;

AxisGizmo::AxisGizmo() {

    MeshManager &meshManager = Root::getInstance().getMeshManager();

    auto mesh = meshManager.getEmpty("axisGizmoMesh");
    if(!mesh->hasData()) {
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

        mesh->lock();

        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

        mesh->setVertices(reinterpret_cast<byte *>(vertices), VERTEX_COUNT, vxDesc);
        mesh->setIndices(static_cast<byte *>(indices), INDEX_COUNT);

        mesh->unlock();
    }

    setMesh(mesh);
    setMaterial("axisGizmoMaterial");
}