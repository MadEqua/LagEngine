#include "Mesh.h"

#include "../io/log/LogManager.h"

#include "GpuBufferManager.h"
#include "../Root.h"

#include "VertexData.h"
#include "IndexData.h"

#include "../Types.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 

using namespace Lag;

Mesh::Mesh() :
	vertexData(nullptr), indexData(nullptr)
{
}

Mesh::~Mesh()
{
	if (vertexData != nullptr)
		delete vertexData;
	if (indexData != nullptr)
		delete indexData;
}

bool Mesh::load()
{
	/*Assimp::Importer importer;
	
	unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;
	
	const aiScene *scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::ERROR,
			"Mesh", "Failed to load mesh from file: " + path + ". Error: " + importer.GetErrorString());
		return;
	}


	uint32 vxCount = 0, idxCount = 0;
	uint8 vxSize = 3 * 4 + 3 * 4 + 3 * 4; //pos, normal and tangent
	for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI)
	{
		aiMesh *mesh = scene->mMeshes[meshI];
		vxCount += mesh->mNumVertices;
		idxCount += mesh->mNumFaces * mesh->mFaces->mNumIndices;

		for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		{
			if (mesh->HasTextureCoords(i))
				vxSize += 3 * 4; //3d coords...
		}
	}



	GpuBufferManager &bufferManager = Root::getInstance().getGpuBufferManager();
	VertexBuffer *vb = bufferManager.createVertexBuffer(vxCount, );
	IndexBuffer *ib = bufferManager.createIndexBuffer();

	for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) 
	{
		aiMesh *mesh = scene->mMeshes[meshI];
		unsigned int nVertices = mesh->mNumVertices;
		unsigned int nIndices = mesh->mNumFaces * mesh->mFaces->mNumIndices;

		IBO *ibo = new IBO(nIndices);
		VBO *vbo = new VBO(nVertices);

		vbo->setAttributeData(0, (void*)mesh->mVertices);

		int normalAttr = vbo->addAttribute(3, sizeof(float));
		vbo->setAttributeData(normalAttr, (void*)mesh->mNormals);

		if (computeTangents) {
			int tangentAttr = vbo->addAttribute(3, sizeof(float));
			vbo->setAttributeData(tangentAttr, (void*)mesh->mTangents);
		}

		if (mesh->HasTextureCoords(0)) {
			int texCoordAttr = vbo->addAttribute(2, sizeof(float));

			for (unsigned int i = 0; i < nVertices; ++i) {
				vbo->setVertexAttributeData(texCoordAttr, i, (void*)&mesh->mTextureCoords[0][i]);
			}
		}
		if (mesh->HasVertexColors(0)) {
			int colorAttr = vbo->addAttribute(3, sizeof(float));
			vbo->setAttributeData(colorAttr, (void*)mesh->mColors[0]);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j) {
				ibo->addIndex(mesh->mFaces[i].mIndices[j]);
			}
		}

		vbo->pushToGPU();
		ibo->pushToGPU();

		vbos[meshI] = vbo;
		ibos[meshI] = ibo;
	}*/


	return false;
}

void Mesh::unload()
{
}