#include "ManagedObject.h"

using namespace Lag;

ManagedObject::ManagedObject() :
	name("Unamed"),
	isLoaded(false)
{
}

bool ManagedObject::load()
{
	if (isLoaded) 
		return true;

	isLoaded = loadImplementation();
	return isLoaded;
}

void ManagedObject::unload()
{
	if(isLoaded)
		unloadImplementation();

	isLoaded = false;
}