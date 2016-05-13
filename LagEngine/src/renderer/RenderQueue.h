#pragma once

#include <vector>

#include "../Types.h"

namespace Lag
{
	class Renderable;
	
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();
 
		//void addRenderable(Renderable &renderable);

		void clear();

		Renderable& getNextSlotToFill();

		void sort();

	private:

		int actualSlot;

		//TODO: find out what's better: vector of pointers or actual Renderables.
		//Pointers are better for sorting, but worse for fast sequential access.
		std::vector<Renderable> queue;
	};
}