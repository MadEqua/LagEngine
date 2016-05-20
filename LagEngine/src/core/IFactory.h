#pragma once

namespace Lag
{
	/*
	* A class implementing this is responsible for the creation of a concrete type.
	* Most likely will need some specific kind of internal parameters related to the construction,
	* allowing the build method to be parameter-less.
	*/
	template<class T>
	class IFactory
	{
	public:
		IFactory() {}
		virtual ~IFactory() {}

		virtual T* create() const = 0;
	};
}