#include "RenderQueue.h"

#include "../io/log/LogManager.h"
#include "Renderable.h"

using namespace Lag;

RenderQueue::RenderQueue() :
	actualSlot(0)
{
	//TODO: something smarter
//	queue.resize(50);
}

RenderQueue::~RenderQueue()
{
}

/*void RenderQueue::addRenderable(Renderable &renderable)
{
	int cap = queue.capacity();
	
	queue.push_back(&renderable);

	if (cap != queue.capacity()) 
	{
		cap = queue.capacity();
		LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::INFO, "RenderQueue", "Queue just got resized. Bad if happening every frame!");
	}
}*/

/*Renderable& RenderQueue::getNextSlotToFill()
{
	if (actualSlot > queue.capacity())
	{
		queue.resize(queue.capacity() * 2);
		LogManager::getInstance().log(CONSOLE, NORMAL, INFO, "RenderQueue", "Queue just got resized. Bad!");
	}
	return queue[actualSlot++];
}*/

void RenderQueue::clear()
{
	//queue.clear();
	//actualSlot = 0;
}

void RenderQueue::sort()
{

}