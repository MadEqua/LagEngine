#pragma once

#include <string>
#include "../Types.h"

namespace Lag
{
	struct InputDescriptionMapKey;
	
	template <class T>
	inline std::string toString(const T &in)
	{
		return std::to_string(in);
	}

	template <>
	inline std::string toString(const std::string &in)
	{
		return in;
	}

	template <>
	inline std::string toString(const InputDescriptionMapKey &in)
	{
		return "InputDescriptionMapKey"; //TODO: something better
	}
}
