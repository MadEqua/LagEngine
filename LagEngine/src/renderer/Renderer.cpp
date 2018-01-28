#include "Renderer.h"

#include <thread>
#include <chrono>

#include "graphicsAPI/RenderToTexture.h"
#include "RenderWindow.h"
#include "../scene/SceneManager.h"
#include "../scene/DirectionalLight.h"
#include "../scene/OrthographicCamera.h"
#include "graphicsAPI/GpuProgram.h"
#include "Material.h"

#include "graphicsAPI/GpuBuffer.h"
#include "VertexData.h"
#include "IndexData.h"
#include "Viewport.h"
#include "graphicsAPI/Texture.h"
#include "graphicsAPI/InputDescription.h"

#include "graphicsAPI/IGraphicsAPI.h"
#include "IFrameListener.h"

#include "../io/log/LogManager.h"

#include "../Root.h"

using namespace Lag;

void TextureBindings::setAsBound(const Texture &tex, uint8 unit)
{
	bindings[unit] = &tex;
}

const Texture* TextureBindings::getBinding(uint8 unit) const
{
	auto it = bindings.find(unit);
	if (it != bindings.end())
		return it->second;
	else
		return nullptr;
}

LAG_DEFINE_NOTIFY_METHOD(Renderer, onFrameStart, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
LAG_DEFINE_NOTIFY_METHOD(Renderer, onFrameRenderingQueued, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))
LAG_DEFINE_NOTIFY_METHOD(Renderer, onFrameEnd, IFrameListener, LAG_ARGS(float timePassed), LAG_ARGS(timePassed))

Renderer::Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager) :
	sceneManager(sceneManager), graphicsAPI(graphicsAPI),
	boundIndexBuffer(nullptr), boundVertexBuffer(nullptr),
	boundGpuProgram(nullptr), boundViewport(nullptr),
	clearColor(126, 192, 238), stencilClearValue(0), depthClearValue(1.0f),
	renderWindowListener(*this),
	actualFrame(0), renderWindow(nullptr),
	lastUsedGpuProgramOnFrame(nullptr)
{
	renderTargets = new NamedContainer<RenderTarget>();
	
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"Renderer", "Initialized successfully.");
}

Renderer::~Renderer()
{
	delete renderTargets;
	delete renderWindow; //must be the last to be deleted
	
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"Renderer", "Destroyed successfully.");
}

void Renderer::startRenderingLoop(uint32 maxFps)
{
	//Min frame time to respect the max FPS
	float minFrameTime = maxFps > 0 ? 1.0f / maxFps : 0.0f;
	
	shouldLoop = true;
	float elapsed = 0.0f;

	while (shouldLoop)
	{
		wholeFrameTimer.start();

		renderWindow->processEvents();

		onFrameStartNotify(frameStartTimer.getElapsedSeconds());
		frameStartTimer.start();

		renderOneFrame();

		onFrameRenderingQueuedNotify(frameQueuedTimer.getElapsedSeconds());
		frameQueuedTimer.start();

		//*At the moment* the render window is the only render target with double buffering.
		renderWindow->swapBuffers();

		onFrameEndNotify(frameEndTimer.getElapsedSeconds());
		frameEndTimer.start();

		elapsed = wholeFrameTimer.getElapsedSeconds();
		if (elapsed < minFrameTime)
		{
			std::chrono::duration<float> dur(minFrameTime - elapsed);
			std::this_thread::sleep_for(dur);
		}
	}
}

void Renderer::stopRenderingLoop()
{
	shouldLoop = false;
}

void Renderer::renderOneFrame()
{
	renderQueue.clear();

	for (uint32 i = 0; i < renderTargets->getSize(); ++i)
	{
		//TODO: add proper iterator
		RenderTarget *rt = renderTargets->get(i);
		if (rt != nullptr)
			rt->addRenderablesToQueue(renderQueue, sceneManager);
	}

	renderWindow->addRenderablesToQueue(renderQueue, sceneManager);

	renderQueue.sort();

	clearColorBuffer();
	clearDepthAndStencilBuffer();


	if (&renderQueue.queue[0].material->getGpuProgram() == lastUsedGpuProgramOnFrame)
		uniformFiller.onGpuProgramBind(lastUsedGpuProgramOnFrame, boundViewport, boundTextures);

	renderQueue.dispatchRenderOperations(*this);

	lastUsedGpuProgramOnFrame = &renderQueue.queue[renderQueue.actualSlot - 1].material->getGpuProgram();

	actualFrame++;
}

uint16 Renderer::addRenderWindow(RenderWindow &renderWindow)
{
	//Only supporting one render window
	if (this->renderWindow != nullptr)
		delete this->renderWindow;

	this->renderWindow = &renderWindow;
	static_cast<RenderTarget&>(renderWindow).registerObserver(renderWindowListener);

	return 0;
}

uint16 Renderer::createRenderToTexture(uint32 width, uint32 height)
{
	RenderToTexture *rtt = graphicsAPI.createRenderToTexture(width, height);
	rtt->initialize();
	return renderTargets->add(static_cast<RenderTarget*>(rtt));
}

void Renderer::removeRenderToTexture(uint16 name)
{
	RenderTarget *rtt = renderTargets->get(name);
	if (rtt != nullptr)
	{
		rtt->destroy();
		renderTargets->remove(name);
	}
}

RenderToTexture* Renderer::getRenderToTexture(uint16 name) const
{
	//TODO: this cast...	
	return static_cast<RenderToTexture*>(renderTargets->get(name));
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

void Renderer::bindRenderTarget(const RenderTarget &renderTarget)
{
	if (&renderTarget != boundRenderTarget)
	{
		boundRenderTarget = &renderTarget;
		graphicsAPI.bindRenderTarget(renderTarget);
	}
}

void Renderer::bindTexture(const Texture &texture, uint8 unit)
{
	const Texture *actualBound = boundTextures.getBinding(unit);
	
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

void Renderer::setDepthTestEnabled(bool enabled)
{
	graphicsAPI.setDepthTestEnabled(enabled);
}

void Renderer::setDepthWritingEnabled(bool enabled)
{
	graphicsAPI.setDepthWritingEnabled(enabled);
}


void Renderer::RenderWindowListener::onResize(RenderTarget &notifier, uint32 width, uint32 height)
{
	if (renderer.boundViewport != nullptr)
	{
		const Viewport *vp = notifier.getViewport(renderer.boundViewport->getName());
		if (renderer.boundViewport == vp)
			renderer.boundViewport = nullptr;
	}
}