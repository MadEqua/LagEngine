#include "Mesh.h"

#include "LogManager.h"

#include "GpuBufferManager.h"
#include "InputDescriptionManager.h"
#include "MemoryBuffer.h"

#include "IndexData.h"
#include "VertexData.h"

#include "SubMesh.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

using namespace Lag;

Mesh::Mesh() : isLocked(false) {
}

Mesh::Mesh(const std::string &file) :
        isLocked(false),
        XmlResource(file) {
}

bool Mesh::loadImplementation() {
    if(!path.empty()) {

        Assimp::Importer importer;

        uint32 flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | aiProcess_Triangulate |
                       aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

        const aiScene *scene = importer.ReadFile(path, flags);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                          "Mesh", "Failed to load mesh from file: " + path + ". Error: " +
                                                  importer.GetErrorString());
            return false;
        }


        //Create the VertexDescription, assuming that all the submeshes will follow the same structure.
        //This allows for all the submeshes to share the same buffer.
        VertexDescription vxDesc;
        vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

        aiMesh *firstMesh = scene->mMeshes[0];
        if (firstMesh->HasNormals())
            vxDesc.addAttribute(VertexAttributeSemantic::NORMAL, 3, VertexAttributeType::FLOAT);

        if (firstMesh->HasTangentsAndBitangents())
            vxDesc.addAttribute(VertexAttributeSemantic::TANGENT, 3, VertexAttributeType::FLOAT);

        for (uint32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
            if (firstMesh->HasTextureCoords(i))
                vxDesc.addAttribute(VertexAttributeSemantic::TEXCOORD, 2, VertexAttributeType::UINT16,
                                    static_cast<uint8>(i), true);
            else break;
        }

        //Make some space calculations
        uint32 vxCount = 0, idxCount = 0;
        uint32 idxSize;
        for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) {
            aiMesh *mesh = scene->mMeshes[meshI];
            vxCount += mesh->mNumVertices;
            idxCount += mesh->mNumFaces * mesh->mFaces->mNumIndices;
        }

        if (idxCount == 0) idxSize = 0;
        else if (idxCount <= MAX_UINT8) idxSize = 1;
        else if (idxCount <= MAX_UINT16) idxSize = 2;
        else idxSize = 4;


        MemoryBuffer vertices(vxCount * vxDesc.getVertexByteSize());
        vertices.lock();

        uint32 vxBufferOffset = 0;
        for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) {
            auto mesh = scene->mMeshes[meshI];
            subMeshes.push_back(std::unique_ptr<SubMesh>(new SubMesh()));

            for (uint32 vx = 0; vx < mesh->mNumVertices; ++vx) {
                uint32 attrSize = vxDesc.getAttribute(VertexAttributeSemantic::POSITION)->getByteSize();
                vertices.write(vxBufferOffset, attrSize, reinterpret_cast<byte *>(&mesh->mVertices[vx]));
                vxBufferOffset += attrSize;

                if (mesh->HasNormals()) {
                    attrSize = vxDesc.getAttribute(VertexAttributeSemantic::NORMAL)->getByteSize();
                    vertices.write(vxBufferOffset, attrSize, reinterpret_cast<byte *>(&mesh->mNormals[vx]));
                    vxBufferOffset += attrSize;
                }

                if (mesh->HasTangentsAndBitangents()) {
                    attrSize = vxDesc.getAttribute(VertexAttributeSemantic::TANGENT)->getByteSize();
                    vertices.write(vxBufferOffset, attrSize, reinterpret_cast<byte *>(&mesh->mTangents[vx]));
                    vxBufferOffset += attrSize;
                }

                for (uint32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
                    if (mesh->HasTextureCoords(i)) {
                        attrSize = vxDesc.getAttribute(VertexAttributeSemantic::TEXCOORD,
                                                       static_cast<uint8>(i))->getByteSize();

                        //Float [0.0, 1.0] to uint16 [0, MAX_UINT16]
                        uint16 texCoord[2];
                        texCoord[0] = static_cast<uint16>(mesh->mTextureCoords[i][vx][0] *static_cast<float>(MAX_UINT16));
                        texCoord[1] = static_cast<uint16>(mesh->mTextureCoords[i][vx][1] *static_cast<float>(MAX_UINT16));

                        vertices.write(vxBufferOffset, attrSize, reinterpret_cast<byte *>(texCoord));
                        vxBufferOffset += attrSize;
                    }
                    else break;
                }
            }

            vertices.unlock();
            setVertices(vertices.getData(), vxCount, vxDesc, meshI);

            if (idxCount > 0) {
                MemoryBuffer indices(idxCount * idxSize);
                indices.lock();

                uint32 idxBufferOffset = 0;
                for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                    for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j) {
                        indices.write(idxBufferOffset, idxSize, reinterpret_cast<byte *>(&mesh->mFaces[i].mIndices[j]));
                        idxBufferOffset += idxSize;
                    }
                }
                indices.unlock();
                setIndices(indices.getData(), idxCount, meshI);
            }
        }
    }

    return true;
}

void Mesh::unloadImplementation() {
    subMeshes.clear();
}

void Mesh::lock() {
    if (!isLocked) {
        subMeshes.clear();
        subMeshes.push_back(std::unique_ptr<SubMesh>(new SubMesh()));
        isLocked = true;
    }
}

void Mesh::unlock() {
    if (isLocked) {
        isLocked = false;

        if(subMeshes[0]->vertexData->vertexCount <= 0) {
            LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Mesh",
                                          "Unlocking a Mesh with no vertices.");
        }
    }
}

void Mesh::setVertices(const MemoryBuffer &vertices, uint32 vertexCount, const VertexDescription &vertexDescription) {
    if (isLocked) {
        setVertices(vertices.getData(), vertexCount, vertexDescription, 0);
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Mesh",
                                      "Trying to set vertices while not locked.");
    }
}

void Mesh::setVertices(const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription) {
    if (isLocked) {
        setVertices(vertices, vertexCount, vertexDescription, 0);
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Mesh",
                                      "Trying to set vertices while not locked.");
    }
}

template<typename T>
void Mesh::setIndices(const std::vector<T> &indices) {
    if (isLocked) {
        setIndices(indices.data(), static_cast<uint32>(indices.size()), 0);
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Mesh",
                                      "Trying to set indices while not locked.");
    }
}

void Mesh::setIndices(const byte *indices, uint32 indexCount) {
    if (isLocked) {
        setIndices(indices, indexCount, 0);
    }
    else {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Mesh",
                                      "Trying to set indices while not locked.");
    }
}


void Mesh::setVertices(const byte *vertices, uint32 vertexCount, const VertexDescription &vertexDescription, uint32 subMeshIndex) {
    GpuBufferManager &bufferManager = Root::getInstance().getGpuBufferManager();
    InputDescriptionManager &inputDescriptionManager = Root::getInstance().getInputDescriptionManager();

    //Create Vertex Buffer on the GPU
    auto &bufferBuilder = dynamic_cast<GpuBufferBuilder &>(bufferManager.getBuilder());
    bufferBuilder.contents = GpuBufferContents::VERTICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = vertexCount;
    bufferBuilder.itemSizeBytes = vertexDescription.getVertexByteSize();
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> vertexBufffer = bufferManager.get();

    //Write data to buffer
    vertexBufffer->lock();
    vertexBufffer->write(0, vertexCount * vertexDescription.getVertexByteSize(), vertices);
    vertexBufffer->unlock();

    //Create VertexData to encapsulate all the vertex data
    auto *vertexData = new VertexData();
    vertexData->inputDescription = inputDescriptionManager.get(vertexDescription, vertexBufffer);
    vertexData->vertexCount = vertexCount;
    vertexData->vertexStart = 0;

    //Give the submesh the ownership of the VertexData
    auto &subMesh = *subMeshes[subMeshIndex];
    subMesh.vertexData = std::unique_ptr<VertexData>(vertexData);
}

void Mesh::setIndices(const byte *indices, uint32 indexCount, uint32 subMeshIndex) {
    GpuBufferManager &bufferManager = Root::getInstance().getGpuBufferManager();

    uint32 idxByteSize;
    IndexType indexType;
    if (indexCount == 0) {
        idxByteSize = 0;
        indexType = IndexType::UINT8;
    }
    else if (indexCount <= MAX_UINT8) {
        idxByteSize = 1;
        indexType = IndexType::UINT8;
    }
    else if (indexCount <= MAX_UINT16) {
        idxByteSize = 2;
        indexType = IndexType::UINT16;
    }
    else {
        idxByteSize = 4;
        indexType = IndexType::UINT32;
    }

    //Create Index Buffer on the GPU
    auto &bufferBuilder = dynamic_cast<GpuBufferBuilder &>(bufferManager.getBuilder());
    bufferBuilder.contents = GpuBufferContents::INDICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = indexCount;
    bufferBuilder.itemSizeBytes = idxByteSize;
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> indexBuffer = bufferManager.get();

    //Write data to buffer
    indexBuffer->lock();
    indexBuffer->write(0, indexCount * idxByteSize, indices);
    indexBuffer->unlock();

    //Create IndexData to encapsulate all the index data
    auto *indexData = new IndexData();
    indexData->indexBuffer = indexBuffer;
    indexData->indexCount = indexCount;
    indexData->indexType = indexType;
    indexData->indexStart = 0;

    //Give the submesh the ownership of the IndexData
    auto &subMesh = *subMeshes[subMeshIndex];
    subMesh.indexData = std::unique_ptr<IndexData>(indexData);
}