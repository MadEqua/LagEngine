#include "IniSettings.h"

#include <sstream>

using namespace Lag;

template bool IniSettings::getFieldAsBasicType<bool>(const std::string &fieldName, bool defaultValue);
template int IniSettings::getFieldAsBasicType<int>(const std::string &fieldName, int defaultValue);
template float IniSettings::getFieldAsBasicType<float>(const std::string &fieldName, float defaultValue);

IniSettings::IniSettings()
{
}

IniSettings::~IniSettings()
{
}

bool IniSettings::containsField(const std::string &name)
{
	return fields.find(name) != fields.end();
}

template<class T>
T IniSettings::getFieldAsBasicType(const std::string &fieldName, T defaultValue)
{
	auto it = fields.find(fieldName);
	if (it != fields.end())
	{
		std::istringstream iss(it->second);
		T value;
		iss >> value;
		return value;
	}
	else
		return defaultValue;
}

const std::string& IniSettings::getFieldAsString(const std::string &fieldName, const std::string &defaultValue)
{
	auto it = fields.find(fieldName);
	if (it != fields.end())
		return it->second;
	else
		return defaultValue;
}

void IniSettings::addField(const std::string &name, const std::string &value)
{
	fields[name] = value;
}
