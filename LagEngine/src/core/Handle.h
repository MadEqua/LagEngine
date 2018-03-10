#pragma once

#include "../Types.h"
#include "ObserverPattern.h"

namespace Lag
{	
	class IObjectZeroReferencesListener
	{
	public:
		virtual ~IObjectZeroReferencesListener() = default;
		virtual void onZeroReferences() = 0;
	};
	

	template<class V>
	class ControlBlock
	{
	public:
		explicit ControlBlock(V* const pointer);
		inline V* const getPointer() const { return pointer; }
		inline void incrementRefCount() { refCount++; }
		void decrementRefCount();

		inline bool hasReferences() const { return refCount > 0; }

		LAG_GENERATE_SINGLE_OBSERVER_STORAGE(IObjectZeroReferencesListener)
		LAG_GENERATE_SINGLE_NOTIFY_METHOD(onZeroReferences, IObjectZeroReferencesListener, LAG_ARGS(), LAG_ARGS())

	private:
		V* const pointer;
		uint32 refCount;
	};


	/*
	* Handle to an object of type V.
	* Classes that depend on object V should hold a copy of this Handle. 
	* The object will not be unloaded and destroyed while there are alive holders.
	* IMPORTANT: Do not hold Handles on classes that should not influence the lifecycle of the handled object.
	*/
	template<class V>
	class Handle
	{ 
	public:
		Handle();
		explicit Handle(ControlBlock<V> &controlBlock);
		Handle(const Handle<V>& other);
		~Handle();

		Handle<V>& operator=(const Handle<V>& other);

		inline void release() { controlBlock->decrementRefCount(); }

		inline V& operator*() const { return *controlBlock->getPointer(); }
		inline V* operator->() const { return controlBlock->getPointer(); }
		inline V* get() const { return controlBlock->getPointer(); }

		inline bool isValid() const { return controlBlock != nullptr && controlBlock->getPointer() != nullptr; }

	private:
		ControlBlock<V> *controlBlock;
	};

#include "Handle.cpp"
}

