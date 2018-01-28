#include "ManagedObject.h"

#include "../io/log/LogManager.h"

using namespace Lag;

ManagedObject::ManagedObject() :
	refCount(0),
	listener(nullptr),
	name("Unamed")
{
}

bool ManagedObject::load()
{
	if (refCount == 0)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"ManagedObject", "Loading ManagedObject for the first time: " + name);
		
		bool loaded = loadImplementation();
		if(loaded)
			refCount++;
		return loaded;
	}
	else
	{
		refCount++;
		return true;
	}
}

void ManagedObject::unload()
{
	refCount--;
	for (ManagedObject *dep : dependencies)
		dep->unload();

	if (refCount <= 0)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"ManagedObject", "Unloading ManagedObject, there are no more references: " + name);

		unloadImplementation();
		notifyZeroReferences();
	}
}

void Lag::ManagedObject::notifyZeroReferences()
{
	if (listener != nullptr)
		listener->onZeroReferences(*this);
}
