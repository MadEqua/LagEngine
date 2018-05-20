#pragma once

#include <string>
#include <sstream>

class TiXmlNode;

namespace Lag
{
	struct InputDescriptionMapKey;

	namespace Utils
	{
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
			return "InputDescriptionMapKey";
		}

		char getPathSeparator();
		std::string getFileNameFromPath(const std::string &path, char separator = '/');
		std::string getDirNameFromPath(const std::string &path, char separator = '/');
		std::string getExtensionFromFile(const std::string &file);

		void copyTinyXmlNode(TiXmlNode *dest, const TiXmlNode *src);
	}
}