#pragma once

#include "../Types.h"

namespace Lag
{
	template<class V>
	class ControlBlock
	{
	public:
		explicit ControlBlock(V* const pointer);
		inline V* const getPointer() const { return pointer; }
		inline void incrementRefCount() { refCount++; }
		inline void decrementRefCount() { refCount--; }
		inline bool hasReferences() const { return refCount > 0; }

	private:
		V* const pointer;
		uint32 refCount;
	};

	/*
	* Handle to an object of type V.
	* Classes that depend on object V should hold a copy of this Handle. 
	* The object will not be unloaded and destroyed while there are alive holders.
	* IMPORTANT: Do not hold a Handle on a class that should not influence the lifecycle of the object.
	*/
	template<class V>
	class Handle
	{ 
	public:
		Handle() : Handle(*(new ControlBlock<V>(nullptr))) {} //TODO: for now allowing empty handles (and leaks)...
		explicit Handle(ControlBlock<V> &controlBlock);
		Handle(const Handle<V>& other);

		~Handle();

		Handle<V>& operator=(const Handle<V>& other);

		inline void release() { controlBlock->decrementRefCount(); }

		inline V& operator*() const { return *controlBlock->getPointer(); }
		inline V* operator->() const { return controlBlock->getPointer(); }
		inline V* get() const { return controlBlock->getPointer(); }

		inline bool isValid() const { return controlBlock->getPointer() != nullptr; }

	private:
		ControlBlock<V> *controlBlock;
	};

#include "Handle.cpp"
}

