#include "RenderQueue.h"

#include "../io/log/LogManager.h"
#include "IRenderable.h"
#include "Renderer.h"
#include "../IFrameListener.h"

using namespace Lag;

RenderQueue::RenderQueue() :
	actualSlot(0)
{
	//TODO: something smarter
	queue.resize(50);
}

RenderQueue::~RenderQueue()
{
}

void RenderQueue::addRenderOperation(IRenderable &renderable, uint32 passId,
	VertexData &vertexData, IndexData *indexData, Material &material,
	Viewport &viewport)
{
	if (actualSlot >= queue.size())
	{
		queue.resize(queue.size() * 2);
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"RenderQueue", "Queue just got resized. Bad Bad!");
	}
	
	RenderOperation &renderOperation = queue[actualSlot];
	renderOperation.renderable = &renderable;
	renderOperation.passId = passId;
	renderOperation.vertexData = &vertexData;
	renderOperation.indexData = indexData;
	renderOperation.material = &material;
	renderOperation.viewport = &viewport;
	
	++actualSlot;
}

void RenderQueue::clear()
{
	actualSlot = 0;
}

void RenderQueue::sort()
{
	//TODO
}

void RenderQueue::dispatchRenderOperations(Renderer &renderer)
{
	for (uint32 i = 0; i < actualSlot; ++i)
	{
		RenderOperation &ro = queue[i];
		IRenderable *renderable = ro.renderable;

		//TODO: render target?
		renderer.bindViewport(*ro.viewport);
		renderable->render(renderer, ro);
	}
}