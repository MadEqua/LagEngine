#pragma once

#include "Utils.h"

using namespace Lag;

char Utils::getPathSeparator()
{
#if defined _WIN32 || defined __CYGWIN__
	return '\\';
#else
	return '/';
#endif
}

std::string Utils::getFileNameFromPath(const std::string &path, char separator)
{
	size_t slash = path.find_last_of(separator);
	if (slash == std::string::npos)
		return std::string();
	else
		return path.substr(slash + 1, std::string::npos);
}

std::string Utils::getDirNameFromPath(const std::string &path, char separator)
{
	size_t slash = path.find_last_of(separator);
	if (slash == std::string::npos)
		return path;
	else
		return path.substr(0, slash);
}

std::string Utils::getExtensionFromFile(const std::string &file)
{
	size_t dot = file.find_last_of('.');
	if (dot == std::string::npos)
		return std::string();
	else
		return file.substr(dot, std::string::npos);
}