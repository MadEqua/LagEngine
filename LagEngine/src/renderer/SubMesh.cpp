#include "SubMesh.h"

#include "VertexDescription.h"
#include "LogManager.h"
#include  "Utils.h"

#include "glm/glm.hpp"

using namespace Lag;

SubMesh::SubMesh(VertexData &vxData, IndexData &idxData) :
        vertexData(&vxData), indexData(&idxData) {
}

SubMesh::SubMesh(VertexData &vxData) :
        vertexData(&vxData) {
}

void SubMesh::updateAABB(const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription) {
    const VertexAttribute *posAttribute = vertexDescription.getAttribute(VertexAttributeSemantic::POSITION);
    if(posAttribute) {
        glm::vec3 pos;

        //The byte size of the type of a vertex position (eg: 4 if a position is in floats)
        uint8 typeSize = posAttribute->getTypeByteSize();

        for(uint32 vx = 0; vx < vertexCount; ++vx) {
            uint32 vxOffset = vx * posAttribute->getByteSize();

            for(uint32 component = 0; component < 3; ++component) {
                uint32 componentOffset = component * typeSize;

                const byte *componentPtr = vertices + vxOffset + componentOffset;

                if(posAttribute->getIsNormalized())
                    pos[component] = Utils::convertNormalizedIntegerToFloat(componentPtr, posAttribute->getType());
                else
                    pos[component] = Utils::convertValueToFloat(componentPtr, posAttribute->getType());
            }

            aabb.enclose(pos);
        }
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "SubMesh", 
                                      "Trying to update an AABB with vertex data with no position.");
    }
}
