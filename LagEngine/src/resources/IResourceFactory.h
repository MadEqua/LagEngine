#pragma once

#include "../core/IFactory.h"

namespace Lag
{
	class Resource;
	
	/*
	* Convenience class to make template/inheritance relations easier.
	*/
	class IResourceFactory : public IFactory<Resource>
	{
	public:
		IResourceFactory() {}
		virtual ~IResourceFactory() {}

		virtual Resource* create() const = 0;
	};
}