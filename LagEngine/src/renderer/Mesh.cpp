#include "Mesh.h"

#include "LogManager.h"

#include "GpuBufferManager.h"
#include "InputDescriptionManager.h"

#include "IndexData.h"
#include "VertexData.h"

#include "SubMesh.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

using namespace Lag;

Mesh::Mesh(const std::string &file) :
        XmlResource(file) {
}

bool Mesh::loadImplementation() {
    Assimp::Importer importer;

    uint32 flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices |
                   aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    const aiScene *scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Mesh", "Failed to load mesh from file: " + path + ". Error: " +
                                              importer.GetErrorString());
        return false;
    }

    GpuBufferManager &bufferManager = Root::getInstance().getGpuBufferManager();
    InputDescriptionManager &inputDescriptionManager = Root::getInstance().getInputDescriptionManager();

    //Create the VertexDescription
    VertexDescription vxDesc;
    vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

    if (scene->mMeshes[0]->HasNormals())
        vxDesc.addAttribute(VertexAttributeSemantic::NORMAL, 3, VertexAttributeType::FLOAT);

    if (scene->mMeshes[0]->HasTangentsAndBitangents())
        vxDesc.addAttribute(VertexAttributeSemantic::TANGENT, 3, VertexAttributeType::FLOAT);

    //Make some space calculations
    uint32 vxCount = 0, idxCount = 0;
    uint32 vxSize, idxSize;
    for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) {
        aiMesh *mesh = scene->mMeshes[meshI];
        vxCount += mesh->mNumVertices;
        idxCount += mesh->mNumFaces * mesh->mFaces->mNumIndices;

        for (uint32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
            if (mesh->HasTextureCoords(i))
                vxDesc.addAttribute(VertexAttributeSemantic::TEXCOORD, 2, VertexAttributeType::UINT16,
                                    static_cast<uint8>(i), true);
            else break;
        }
    }

    vxSize = vxDesc.getByteSize();

    if (idxCount == 0) idxSize = 0;
    else if (idxCount <= MAX_UINT8) idxSize = 1;
    else if (idxCount <= MAX_UINT16) idxSize = 2;
    else idxSize = 4;

    //create buffers
    auto &bufferBuilder = dynamic_cast<GpuBufferBuilder &>(bufferManager.getBuilder());
    bufferBuilder.contents = GpuBufferContents::VERTICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = vxCount;
    bufferBuilder.itemSizeBytes = vxSize;
    bufferBuilder.useMirrorBuffer = true;
    Handle<GpuBuffer> vb = bufferManager.get();

    if (!vb.isValid()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Mesh",
                                      "Failed to load mesh from file: " + path + ". Failed to build a VertexBuffer");
        return false;
    }

    Handle<GpuBuffer> ib;
    if (idxCount > 0) {
        bufferBuilder.contents = GpuBufferContents::INDICES;
        bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
        bufferBuilder.itemCount = idxCount;
        bufferBuilder.itemSizeBytes = idxSize;
        bufferBuilder.useMirrorBuffer = true;
        ib = bufferManager.get();

        if (!ib.isValid()) {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                          "Mesh",
                                          "Failed to load mesh from file: " + path + ". Failed to build a IndexBuffer");
            return false;
        }
    }

    //for each submesh
    uint32 vxBufferOffset = 0, idxBufferOffset = 0;
    for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) {
        aiMesh *mesh = scene->mMeshes[meshI];
        uint32 subMeshVxCount = mesh->mNumVertices;
        uint32 subMeshIdxCount = mesh->mNumFaces * mesh->mFaces->mNumIndices;

        //Fill vertex buffer with the respective submesh part (interleaved)
        uint32 posSize = vxDesc.getAttribute(VertexAttributeSemantic::POSITION)->getByteSize();
        uint32 norSize = 0;
        uint32 tanSize = 0;

        if (mesh->HasNormals())
            norSize = vxDesc.getAttribute(VertexAttributeSemantic::NORMAL)->getByteSize();
        if (mesh->HasTangentsAndBitangents())
            tanSize = vxDesc.getAttribute(VertexAttributeSemantic::TANGENT)->getByteSize();

        vb->lock(vxBufferOffset, vxSize * subMeshVxCount);
        uint32 offset = 0;
        for (uint32 vx = 0; vx < mesh->mNumVertices; ++vx) {
            vb->write(offset, posSize, reinterpret_cast<byte *>(&mesh->mVertices[vx]));
            offset += posSize;

            if (mesh->HasNormals()) {
                vb->write(offset, norSize, reinterpret_cast<byte *>(&mesh->mNormals[vx]));
                offset += norSize;
            }

            if (mesh->HasTangentsAndBitangents()) {
                vb->write(offset, tanSize, reinterpret_cast<byte *>(&mesh->mTangents[vx]));
                offset += tanSize;
            }

            for (uint32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
                if (mesh->HasTextureCoords(i)) {
                    uint32 texCoordSize = vxDesc.getAttribute(VertexAttributeSemantic::TEXCOORD,
                                                              static_cast<uint8>(i))->getByteSize();

                    //Float [0.0, 1.0] to uint16 [0, MAX_UINT16]
                    uint16 texCoord[2];
                    texCoord[0] = static_cast<uint16>(mesh->mTextureCoords[i][vx][0] * static_cast<float>(MAX_UINT16));
                    texCoord[1] = static_cast<uint16>(mesh->mTextureCoords[i][vx][1] * static_cast<float>(MAX_UINT16));

                    vb->write(offset, texCoordSize, reinterpret_cast<byte *>(texCoord));
                    offset += texCoordSize;
                }
                else break;
            }
        }
        vb->unlock();
        vb->setUseMirror(false);

        //fill index buffer (writes are done to the in-memory mirror buffer)
        if (idxCount > 0) {
            ib->lock();
            offset = 0;
            for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
                for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j) {
                    ib->write(offset, idxSize, reinterpret_cast<byte *>(mesh->mFaces[i].mIndices + j));
                    offset += idxSize;
                }
            ib->unlock();
        }

        ib->setUseMirror(false);

        //Create Data objects, will be managed by the SubMesh
        auto *vd = new VertexData();

        vd->inputDescription = inputDescriptionManager.get(vxDesc, vb);
        vd->vertexCount = mesh->mNumVertices;
        vd->vertexStart = vxBufferOffset;

        IndexData *id = nullptr;
        if (idxCount > 0) {
            id = new IndexData();
            id->indexBuffer = ib;
            id->indexCount = subMeshIdxCount;

            if (idxSize <= 1) id->indexType = IndexType::UINT8;
            else if (idxSize <= 2) id->indexType = IndexType::UINT16;
            else id->indexType = IndexType::UINT32;

            id->indexStart = idxBufferOffset;
        }

        //Create SubMeshes
        //TODO: add textures and send to SubMesh
        //std::vector<Texture*> textures;
        SubMesh *subMesh;
        if (idxCount > 0)
            subMesh = new SubMesh(*vd, *id);
        else
            subMesh = new SubMesh(*vd);

        subMeshes.push_back(subMesh);

        vxBufferOffset += vxSize * subMeshVxCount;
        idxBufferOffset += idxSize * subMeshIdxCount;
    }

    return true;
}

void Mesh::unloadImplementation() {
    for (SubMesh *sm : subMeshes)
        delete sm;
}