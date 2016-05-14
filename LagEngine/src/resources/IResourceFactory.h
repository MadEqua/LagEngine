#pragma once

namespace Lag
{
	class Resource;
	
	/*
	* A class implementing this is responsible for the creation of a Resource type.
	* Most likely will need some specific kind of internal parameters related to the construction, 
	* allowing the build method to be parameter-less.
	*/
	class IResourceFactory
	{
	public:
		IResourceFactory() {}
		virtual ~IResourceFactory() {}

		virtual Resource* create() const = 0;
	};
}