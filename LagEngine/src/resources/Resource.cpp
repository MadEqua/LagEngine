#include "Resource.h"

using namespace Lag;

Resource::Resource(const std::string &path) :
	loaded(false),
	path(path)
{
}

Resource::Resource() :
	loaded(false)
{
}

Resource::~Resource()
{
}