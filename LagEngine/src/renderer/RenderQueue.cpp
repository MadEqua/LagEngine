#include "RenderQueue.h"

#include "../io/log/LogManager.h"
#include "IRenderable.h"
#include "Renderer.h"
#include "Material.h"

using namespace Lag;

RenderQueue::RenderQueue() :
	currentSlot(0)
{
	//TODO: something smarter
	queue.resize(50);
}

RenderOperation& RenderQueue::addRenderOperation()
{
	if (currentSlot >= queue.size())
	{
		queue.resize(queue.size() * 2);
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"RenderQueue", "Queue just got resized. Bad Bad!");
	}

	return queue[currentSlot++];
}

/*void RenderQueue::addRenderOperation(IRenderable &renderable, RenderPhase renderPhase, uint32 passId,
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
	renderOperation.renderPhase = renderPhase;
	renderOperation.passId = passId;
	renderOperation.vertexData = &vertexData;
	renderOperation.indexData = indexData;
	renderOperation.material = &material;
	renderOperation.viewport = &viewport;
	
	++actualSlot;
}*/

void RenderQueue::clear()
{
	currentSlot = 0;
}

void RenderQueue::sort()
{
	//TODO
}

void RenderQueue::dispatchRenderOperations(Renderer &renderer)
{
	for (uint32 i = 0; i < currentSlot; ++i)
	{
		RenderOperation &ro = queue[i];
		IRenderable *renderable = ro.renderable;

		renderer.setRenderMode(ro.renderMode);

		//Bind GpuProgram and then the Textures
		ro.material->bind();
		renderer.bindRenderTarget(*ro.renderTarget);
		renderer.bindViewport(*ro.viewport);
		renderable->render(renderer, ro);
	}
}