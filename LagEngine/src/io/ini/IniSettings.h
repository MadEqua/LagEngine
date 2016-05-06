#pragma once

#include <string>
#include <unordered_map>

namespace Lag
{
	class IniSettings
	{
	public:
		IniSettings();
		~IniSettings();

		bool containsField(const std::string &name);

		template <class T>
		T getFieldAsBasicType(const std::string &fieldName, T defaultValue = T());

		const std::string& getFieldAsString(const std::string &fieldName, const std::string &defaultValue = "");

		void addField(const std::string &name, const std::string &value);

	private:
		std::unordered_map<std::string, std::string> fields;
	};
}

