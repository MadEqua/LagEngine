#include "ManagedObject.h"

using namespace Lag;

ManagedObject::ManagedObject() : 
	loaded(false)
{
}

bool ManagedObject::load()
{
	if (!loaded)
	{
		loaded = loadImplementation();
		return loaded;
	}
	else
		return true;
}

void ManagedObject::unload()
{
	if (loaded)
	{
		unloadImplementation();
		loaded = false;
	}
}