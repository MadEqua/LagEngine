#include "Resource.h"

using namespace Lag;

Resource::Resource(const std::string &path) :
	isLoaded(false),
	path(path)
{
}

Resource::Resource() :
	isLoaded(false)
{
}

Resource::~Resource()
{
}