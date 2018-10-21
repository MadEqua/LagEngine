#include "RenderQueue.h"

#include "LogManager.h"
#include "IRenderable.h"
#include "Renderer.h"
#include "Material.h"

using namespace Lag;

RenderQueue::RenderQueue() :
        currentSlot(0) {
    //TODO: something smarter
    queue.resize(50);
}

RenderOperation &RenderQueue::addRenderOperation() {
    if (currentSlot >= queue.size()) {
        queue.resize(queue.size() * 2);
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
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
		LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
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

void RenderQueue::clear() {
    currentSlot = 0;
}

void RenderQueue::sort() {
    //TODO
}

void RenderQueue::dispatchRenderOperations(Renderer &renderer) {
    for (uint32 i = 0; i < currentSlot; ++i) {
        RenderOperation &ro = queue[i];
        IRenderable *renderable = ro.renderable;

        renderer.bindRenderTarget(*ro.renderTarget);
        renderer.bindViewport(*ro.viewport);

        //Bind GpuProgram and then the Textures
        ro.material->bind();

        renderable->preRender(renderer, ro);
        renderable->render(renderer, ro);
    }
}