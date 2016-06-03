#include "Renderer.h"

#include "RenderTarget.h"
#include "../scene/SceneManager.h"
#include "graphicsAPI/GpuProgram.h"
#include "Material.h"

#include "graphicsAPI/GpuBuffer.h"
#include "VertexData.h"
#include "IndexData.h"
#include "Viewport.h"
#include "graphicsAPI/Texture.h"
#include "graphicsAPI/InputDescription.h"

#include "graphicsAPI/IGraphicsAPI.h"

#include "../io/log/LogManager.h"

#include "../Root.h"

using namespace Lag;

bool TextureBindings::MapKey::operator==(const MapKey &other) const
{
	return type == other.type && unit == other.unit;
}

std::size_t TextureBindings::MapKey::MapKeyHasher::operator()(const MapKey& k) const
{
	return std::hash<TextureType>()(k.type) ^ std::hash<uint8>()(k.unit);
}

void TextureBindings::setAsBound(const Texture &tex, uint8 unit)
{
	MapKey key(tex.getType(), unit);
	bindings[key] = &tex;
}

const Texture* TextureBindings::getBinding(TextureType type, uint8 unit) const
{
	MapKey key(type, unit);
	auto it = bindings.find(key);
	if (it != bindings.end())
		return it->second;
	else
		return nullptr;
}


Renderer::Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager) :
	sceneManager(sceneManager), graphicsAPI(graphicsAPI),
	boundIndexBuffer(nullptr), boundVertexBuffer(nullptr),
	boundGpuProgram(nullptr), boundViewport(nullptr),
	clearColor(0.5f), stencilClearValue(0), depthClearValue(1.0f),
	renderTargetListener(*this),
	actualFrame(0),
	lastUsedGpuProgramOnFrame(nullptr)
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
	renderTarget.registerObserver(renderTargetListener);
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

	clearColorBuffer();
	clearDepthAndStencilBuffer();


	if (&renderQueue.queue[0].material->getGpuProgram() == lastUsedGpuProgramOnFrame)
		uniformFiller.onGpuProgramBind(lastUsedGpuProgramOnFrame, boundViewport, boundTextures);

	renderQueue.dispatchRenderOperations(*this);

	lastUsedGpuProgramOnFrame = &renderQueue.queue[renderQueue.actualSlot-1].material->getGpuProgram();

	actualFrame++;
}

void Renderer::bindVertexBuffer(const GpuBuffer &vertexBuffer)
{ 
	if (&vertexBuffer != boundVertexBuffer)
	{
		boundVertexBuffer = &vertexBuffer;
		graphicsAPI.bindVertexBuffer(vertexBuffer);
	}
}

void Renderer::bindIndexBuffer(const GpuBuffer &indexBuffer)
{ 
	if(&indexBuffer != boundIndexBuffer)
	{
		boundIndexBuffer = &indexBuffer;
		graphicsAPI.bindIndexBuffer(indexBuffer);
	}
}

void Renderer::bindGpuProgram(const GpuProgram &gpuProgram)
{ 
	if(&gpuProgram != boundGpuProgram)
	{
		boundGpuProgram = &gpuProgram;
		graphicsAPI.bindGpuProgram(gpuProgram);
		uniformFiller.onGpuProgramBind(boundGpuProgram, boundViewport, boundTextures);
	}
}

void Renderer::bindInputDescription(const InputDescription &inputDescription)
{
	if (&inputDescription != boundInputDescription)
	{
		boundInputDescription = &inputDescription;
		graphicsAPI.bindInputDescription(inputDescription);
	}
}

void Renderer::bindViewport(const Viewport &viewport)
{
	if (&viewport != boundViewport)
	{
		boundViewport = &viewport;
		graphicsAPI.bindViewport(viewport.getRealLeft(), viewport.getRealBottom(),
			viewport.getRealWidth(), viewport.getRealHeight());
		uniformFiller.onViewportBind(boundGpuProgram, boundViewport);
	}
}

void Renderer::bindTexture(const Texture &texture, uint8 unit)
{
	const Texture *actualBound = boundTextures.getBinding(texture.getType(), unit);
	
	if (&texture != actualBound)
	{
		boundTextures.setAsBound(texture, unit);
		graphicsAPI.bindTexture(texture, unit);
		uniformFiller.onTextureBind(boundGpuProgram, &texture, unit);
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

void Renderer::setClearColor(const Color &color)
{
	clearColor = color;
}

void Renderer::setClearDepthValue(float value)
{
	depthClearValue = value;
}

void Renderer::setClearStencilValue(int32 value)
{
	stencilClearValue = value;
}

void Renderer::clearColorBuffer()
{
	graphicsAPI.clearColorBuffer(clearColor);
}

void Renderer::clearDepthBuffer()
{
	graphicsAPI.clearDepthBuffer(depthClearValue);
}

void Renderer::clearStencilBuffer()
{
	graphicsAPI.clearStencilBuffer(stencilClearValue);
}

void Renderer::clearDepthAndStencilBuffer()
{
	graphicsAPI.clearDepthAndStencilBuffer(depthClearValue, stencilClearValue);
}


void Renderer::RenderTargetListener::onResize(RenderTarget &notifier, int width, int height)
{
	if (renderer.boundViewport != nullptr)
	{
		const Viewport *vp = notifier.getViewport(renderer.boundViewport->getName());
		if (renderer.boundViewport == vp)
			renderer.boundViewport = nullptr;
	}
}