#include "Renderer.h"

#include "RenderTarget.h"
#include "../scene/SceneManager.h"
#include "graphicsAPI/GpuProgram.h"

#include "graphicsAPI/VertexBuffer.h"
#include "graphicsAPI/IndexBuffer.h"

#include "VertexData.h"
#include "IndexData.h"

#include "graphicsAPI/IGraphicsAPI.h"

#include "../io/log/LogManager.h"

using namespace Lag;

Renderer::Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager) :
	sceneManager(sceneManager), graphicsAPI(graphicsAPI),
	boundIndexBuffer(nullptr), boundVertexBuffer(nullptr),
	boundGpuProgram(nullptr)
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"Renderer", "Initialized successfully.");
}

Renderer::~Renderer()
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"Renderer", "Destroyed successfully.");
}

void Renderer::addRenderTarget(const std::string &name, RenderTarget &renderTarget)
{
	renderTargets[name] = &renderTarget;
}

void Renderer::removeRenderTarget(const std::string &name)
{
	renderTargets.erase(name);
}

void Renderer::renderAllRenderTargets()
{
	renderQueue.clear();
	
	for (auto &rtPair : renderTargets)
	{
		RenderTarget &rt = *rtPair.second;
		rt.addRenderablesToQueue(renderQueue, sceneManager);
	}

	renderQueue.sort();
	renderQueue.dispatchRenderOperations(graphicsAPI);
}

void Renderer::bindVertexBuffer(VertexBuffer &vertexBuffer)
{ 
	if (&vertexBuffer != boundVertexBuffer)
	{
		boundVertexBuffer = &vertexBuffer;
		vertexBuffer.bind();
	}
}

void Renderer::bindIndexBuffer(IndexBuffer &indexBuffer)
{ 
	if(&indexBuffer != boundIndexBuffer)
	{
		boundIndexBuffer = &indexBuffer;
		indexBuffer.bind();
	}
}

void Renderer::bindGpuProgram(GpuProgram &gpuProgram)
{ 
	if(&gpuProgram != boundGpuProgram)
	{
		boundGpuProgram = &gpuProgram;
		gpuProgram.bind();
	}
}

void Renderer::renderVertices(const VertexData &vertexData)
{
	//graphicsAPI.renderVertices(actualRenderMode, vertexData.vertexStart, vertexData.vertexCount);
}

void Renderer::renderIndexed(const VertexData &vertexData, const IndexData &indexData, uint32 baseVertex)
{
	//bind index buffer?
	//graphicsAPI.renderIndexed(actualRenderMode, indexData.indexStart, )
}

void Renderer::renderMultiVertices(const VertexData *vertexData[], uint32 drawCount)
{

}

void Renderer::renderMultiIndexed(const VertexData *vertexData[], const IndexData *indexData[], uint32 drawCount)
{
}

void Renderer::renderVerticesInstanced(const VertexData &vertexData, uint32 instanceCount)
{

}

void Renderer::renderIndexedInstanced(const VertexData &vertexData, const IndexData &indexData, uint32 instanceCount)
{

}