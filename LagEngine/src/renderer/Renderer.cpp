#include "Renderer.h"

#include "RenderTarget.h"
#include "../scene/SceneManager.h"
#include "graphicsAPI/GpuProgram.h"

#include "graphicsAPI/GpuBuffer.h"
#include "VertexData.h"
#include "IndexData.h"
#include "Viewport.h"
#include "graphicsAPI/InputDescription.h"

#include "graphicsAPI/IGraphicsAPI.h"

#include "../io/log/LogManager.h"

using namespace Lag;

Renderer::Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager) :
	sceneManager(sceneManager), graphicsAPI(graphicsAPI),
	boundIndexBuffer(nullptr), boundVertexBuffer(nullptr),
	boundGpuProgram(nullptr), boundViewport(nullptr)
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"Renderer", "Initialized successfully.");
}

Renderer::~Renderer()
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
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

	//TODO: do it right
	float color[4] = { 0.5f, 0.5f, 0.5f, 1 };
	clearColorBuffer(color);
	clearDepthAndStencilBuffer(1.0f, 0);

	renderQueue.dispatchRenderOperations(*this);

	for (auto &rtPair : renderTargets)
	{
		RenderTarget &rt = *rtPair.second;
		rt.swapBuffers();
	}
}

void Renderer::bindVertexBuffer(const GpuBuffer &vertexBuffer)
{ 
	if (&vertexBuffer != boundVertexBuffer)
	{
		boundVertexBuffer = &vertexBuffer;
		vertexBuffer.bind();
	}
}

void Renderer::bindIndexBuffer(const GpuBuffer &indexBuffer)
{ 
	if(&indexBuffer != boundIndexBuffer)
	{
		boundIndexBuffer = &indexBuffer;
		indexBuffer.bind();
	}
}

void Renderer::bindGpuProgram(const GpuProgram &gpuProgram)
{ 
	if(&gpuProgram != boundGpuProgram)
	{
		boundGpuProgram = &gpuProgram;
		gpuProgram.bind();
	}
}

void Renderer::bindInputDescription(const InputDescription &inputDescription)
{
	if (&inputDescription != boundInputDescription)
	{
		boundInputDescription = &inputDescription;
		inputDescription.bind();
	}
}

void Renderer::bindViewport(const Viewport &viewport)
{
	//TODO: fix. won't work if the bound viewport was changed (or its rendertarget size)
	//if (&viewport != boundViewport)
	{
		boundViewport = &viewport;
		graphicsAPI.setViewport(viewport.getRealLeft(), viewport.getRealBottom(),
			viewport.getRealWidth(), viewport.getRealHeight());
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