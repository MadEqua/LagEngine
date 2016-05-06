#pragma once

#include <string>
#include <unordered_map>

#include "../../core/SingletonPattern.h"

namespace Lag
{
	class IniSettings;
	
	class IniManager
	{
		GENERATE_SINGLETON(IniManager)
	
	public:
		bool parse(const std::string &filePath, IniSettings **out);

	private:
		std::unordered_map<std::string, IniSettings*> files;

		std::string trim(std::string &in);
	};
}