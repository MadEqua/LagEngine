
template<class V>
ControlBlock<V>::ControlBlock(V* const pointer) :
	refCount(0),
	pointer(pointer)
{
}

template<class V>
Handle<V>::Handle(ControlBlock<V> &controlBlock) :
	controlBlock(&controlBlock)
{
	this->controlBlock->incrementRefCount();
}

template<class V>
Handle<V>::Handle(const Handle<V>& other) : 
	Handle(*other.controlBlock)
{
}

template<class V>
Lag::Handle<V>::~Handle()
{
	controlBlock->decrementRefCount();
}

template<class V>
Handle<V>& Handle<V>::operator=(const Handle<V>& other)
{
	controlBlock = other.controlBlock;
	controlBlock->incrementRefCount();
	return *this;
}
