#pragma once

#include <string>
#include "IniSettings.h"

namespace Lag
{
	class IniParser
	{
	public:
		IniParser();
		~IniParser();

		bool parse(const std::string &filePath);

		inline const IniSettings& getParsedIniSettings() { return out; }

	private:
		IniSettings out;
		std::string trim(std::string &in) const;
	};
}