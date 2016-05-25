#include "Renderer.h"

#include "RenderTarget.h"
#include "../scene/SceneManager.h"
#include "graphicsAPI/GpuProgram.h"

#include "graphicsAPI/GpuBuffer.h"
#include "VertexData.h"
#include "IndexData.h"
#include "graphicsAPI/InputDescription.h"

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

	float color[4] = { 0.5f, 0.5f, 0.5f, 1 };
	clearColorBuffer(color);
	clearDepthAndStencilBuffer(0.0f, 0);

	renderQueue.dispatchRenderOperations(*this);

	for (auto &rtPair : renderTargets)
	{
		RenderTarget &rt = *rtPair.second;
		rt.swapBuffers();
	}
}

void Renderer::bindVertexBuffer(GpuBuffer &vertexBuffer)
{ 
	if (&vertexBuffer != boundVertexBuffer)
	{
		boundVertexBuffer = &vertexBuffer;
		vertexBuffer.bind();
	}
}

void Renderer::bindIndexBuffer(GpuBuffer &indexBuffer)
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

void Renderer::bindInputDescription(InputDescription &inputDescription)
{
	if (&inputDescription != boundInputDescription)
	{
		boundInputDescription = &inputDescription;
		inputDescription.bind();
	}
}

void Renderer::renderVertices(const VertexData &vertexData)
{
	bindInputDescription(*vertexData.inputDescription);
	graphicsAPI.renderVertices(actualRenderMode, vertexData.vertexStart, vertexData.vertexCount);
}

void Renderer::renderIndexed(const VertexData &vertexData, const IndexData &indexData, uint32 baseVertex)
{
	bindInputDescription(*vertexData.inputDescription);
	bindIndexBuffer(*indexData.indexBuffer);
	graphicsAPI.renderIndexed(actualRenderMode, indexData.indexStart, indexData.indexType, indexData.indexCount, baseVertex);
}

/*void Renderer::renderMultiVertices(const VertexData *vertexData[], uint32 drawCount)
{
	bindInputDescription(*vertexData.inputDescription);
}

void Renderer::renderMultiIndexed(const VertexData *vertexData[], const IndexData *indexData[], uint32 drawCount)
{
	bindInputDescription(*vertexData.inputDescription);
}

void Renderer::renderVerticesInstanced(const VertexData &vertexData, uint32 instanceCount)
{
	bindInputDescription(*vertexData.inputDescription);
}

void Renderer::renderIndexedInstanced(const VertexData &vertexData, const IndexData &indexData, uint32 instanceCount)
{
	bindInputDescription(*vertexData.inputDescription);
}*/

void Renderer::clearColorBuffer(float value[4])
{
	graphicsAPI.clearColorBuffer(value);
}

void Renderer::clearDepthBuffer(float value)
{
	graphicsAPI.clearDepthBuffer(value);
}

void Renderer::clearStencilBuffer(int32 value)
{
	graphicsAPI.clearStencilBuffer(value);
}

void Renderer::clearDepthAndStencilBuffer(float depth, int32 stencil)
{
	graphicsAPI.clearDepthAndStencilBuffer(depth, stencil);
}