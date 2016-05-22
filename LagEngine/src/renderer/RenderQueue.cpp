#include "RenderQueue.h"

#include "../io/log/LogManager.h"
#include "Renderable.h"

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

void RenderQueue::addItem(RenderCommand renderCommand, RenderType renderType,
	VertexData &vertexData, IndexData &indexData,
	ShaderProgram &shaderProgram)
{
	if (actualSlot >= queue.size())
	{
		queue.resize(queue.size() * 2);
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
			"RenderQueue", "Queue just got resized. Bad Bad!");
	}
	
	RenderOperation &renderOperation = queue[actualSlot];
	renderOperation.renderCommand = renderCommand;
	renderOperation.renderType = renderType;
	renderOperation.vertexData = &vertexData;
	renderOperation.indexData = &indexData;
	renderOperation.shaderProgram = &shaderProgram;
	
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