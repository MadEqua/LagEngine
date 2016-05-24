#include "Mesh.h"

#include "../io/log/LogManager.h"

#include "graphicsAPI/GpuBufferManager.h"
#include "graphicsAPI/InputDescriptionManager.h"
#include "../Root.h"

#include "../Types.h"

#include "graphicsAPI/VertexBuffer.h"
#include "graphicsAPI/IndexBuffer.h"
#include "IndexData.h"
#include "VertexData.h"
#include "VertexDescription.h"

#include "SubMesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 

using namespace Lag;

Mesh::Mesh(const std::string &file) :
	Resource(file)
{
}

Mesh::~Mesh()
{
	for (SubMesh *sm : subMeshes)
		delete sm;
}

bool Mesh::loadImplementation()
{
	Assimp::Importer importer;
	
	uint32 flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;
	
	const aiScene *scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR,
			"Mesh", "Failed to load mesh from file: " + path + ". Error: " + importer.GetErrorString());
		return false;
	}

	GpuBufferManager &bufferManager = Root::getInstance().getGpuBufferManager();
	InputDescriptionManager &inputDescriptionManager = Root::getInstance().getInputDescriptionManager();

	//Create a VertexDescription
	VertexDescription &vxDesc = inputDescriptionManager.createVertexDescription();
	vxDesc.addAttribute(LAG_VX_ATTR_SEMANTIC_POSITION, 0, 3 * sizeof(float), LAG_VX_ATTR_TYPE_FLOAT);
	vxDesc.addAttribute(LAG_VX_ATTR_SEMANTIC_NORMAL, 3 * sizeof(float), 3 * sizeof(float), LAG_VX_ATTR_TYPE_FLOAT);
	vxDesc.addAttribute(LAG_VX_ATTR_SEMANTIC_TANGENT, 6 * sizeof(float), 3 * sizeof(float), LAG_VX_ATTR_TYPE_FLOAT);

	//Make some space calculations
	uint32 vxCount = 0, idxCount = 0;
	uint32 vxSize = 3 * sizeof(float) + 3 * sizeof(float) + 3 * sizeof(float); //pos, normal and tangent
	uint32 idxSize;
	for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI)
	{
		aiMesh *mesh = scene->mMeshes[meshI];
		vxCount += mesh->mNumVertices;
		idxCount += mesh->mNumFaces * mesh->mFaces->mNumIndices;

		for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		{
			if (mesh->HasTextureCoords(i))
			{
				vxSize += 3 * sizeof(float); //3d coords...
				vxDesc.addAttribute(LAG_VX_ATTR_SEMANTIC_TEX_COORD, 9 * sizeof(float), 3 * sizeof(float), LAG_VX_ATTR_TYPE_FLOAT, i);
			}
			else
				break;
		}
	}
	if (idxCount == 0) idxSize = 0;
	else if (idxCount < 256) idxSize = 1;
	else if (idxCount < 65536) idxSize = 2;
	else idxSize = 4;

	//create buffers
	VertexBuffer *vb = bufferManager.createVertexBuffer(vxCount, vxSize, false);
	IndexBuffer *ib = nullptr;
	if(idxCount > 0)
		ib = bufferManager.createIndexBuffer(idxCount, idxSize, true);

	//for each submesh
	uint32 vxBufferOffset = 0, idxBufferOffset = 0;
	for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) 
	{
		aiMesh *mesh = scene->mMeshes[meshI];
		uint32 subMeshVxCount = mesh->mNumVertices;
		uint32 subMeshIdxCount = mesh->mNumFaces * mesh->mFaces->mNumIndices;
		
		//Fill buffers with the respective submesh part
		vb->lock(vxBufferOffset, vxSize * subMeshVxCount);
		uint32 offset = 0;
		for (uint32 vx = 0; vx < mesh->mNumVertices; ++vx)
		{
			vb->write(offset, 3 * sizeof(float), (byte*)mesh->mVertices);
			offset += 3 * sizeof(float);

			vb->write(offset, 3 * sizeof(float), (byte*)mesh->mNormals);
			offset += 3 * sizeof(float);

			vb->write(offset, 3 * sizeof(float), (byte*)mesh->mTangents);
			offset += 3 * sizeof(float);

			for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
			{
				if (mesh->HasTextureCoords(i))
				{
					vb->write(offset, 3 * sizeof(float), (byte*)mesh->mTextureCoords[i]);
					offset += 3 * sizeof(float);
				}
				else break;
			}
		}
		vb->unlock();

		if (idxCount > 0)
		{
			ib->lock(idxBufferOffset, subMeshIdxCount);
			byte *ptr = ib->map();
			for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
			{
				for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j)
				{
					switch (idxSize)
					{
					case 1:
						*ptr = mesh->mFaces[i].mIndices[j];
						ptr++;
						break;
					case 2: 
					{
						uint16 *ptr16 = reinterpret_cast<uint16*>(ptr);
						*ptr16 = mesh->mFaces[i].mIndices[j];
						ptr += 2; 
					}
						break;
					case 4:
					{
						uint32 *ptr32 = reinterpret_cast<uint32*>(ptr);
						*ptr32 = mesh->mFaces[i].mIndices[j];
						ptr += 4;
					}
						break;
					default:
						break;
					}
				}
			}
			ib->unmap();
			ib->unlock();
		}
	

		//Create Data objects, will be managed by the SubMesh
		VertexData *vd = new VertexData();
		vd->inputDescription = inputDescriptionManager.getInputDescription(vxDesc, *vb);
		vd->vertexCount = mesh->mNumVertices;
		vd->vertexStart = vxBufferOffset;

		IndexData *id = nullptr;
		if (idxCount > 0)
		{
			id = new IndexData();
			id->indexBuffer = ib;
			id->indexCount = subMeshIdxCount;

			if (idxSize <= 1) id->indexType = LAG_IDX_TYPE_UINT8;
			else if (idxSize <= 2) id->indexType = LAG_IDX_TYPE_UINT16;
			else idxSize = id->indexType = LAG_IDX_TYPE_UINT32;

			id->indexStart = idxBufferOffset;
		}

		//Create SubMeshes
		//TODO: add textures
		std::vector<Texture*> textures;
		SubMesh *subMesh;
		if (idxCount > 0)
			subMesh = new SubMesh(*vd, *id, textures);
		else
			subMesh = new SubMesh(*vd, textures);

		subMeshes.push_back(subMesh);

		vxBufferOffset += vxSize * subMeshVxCount;
		idxBufferOffset += idxSize * subMeshIdxCount;
	}

	return true;
}

void Mesh::unloadImplementation()
{
}