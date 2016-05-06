#include "IniManager.h"

#include <fstream>

#include "../log/LogManager.h"
#include "IniSettings.h"

using namespace Lag;

IniManager::IniManager()
{
	LogManager::getInstance().log(FILE, NORMAL, INFO, "IniManager", "Initialized successfully.");
}

IniManager::~IniManager()
{
	for (const std::pair<std::string, IniSettings*> &p : files)
	{
		delete p.second;
	}
	LogManager::getInstance().log(FILE, NORMAL, INFO, "IniManager", "Destroyed successfully.");
}

bool IniManager::parse(const std::string &filePath, IniSettings **out)
{
	auto it = files.find(filePath);
	if (it != files.end())
	{
		*out = files[filePath];
		return true;
	}
		
	std::ifstream inFile(filePath, std::ios::in);
	if (!inFile.is_open())
	{
		LogManager::getInstance().log(FILE, NORMAL, ERROR, filePath, "Error opening .ini file.");
		*out = nullptr;
		return false;
	}
		
	IniSettings *iniSettings = new IniSettings();
	files[filePath] = iniSettings;
	*out = iniSettings;

	std::string line;
	while (std::getline(inFile, line))
	{
		line = trim(line);
		size_t equalPos = line.find('=');
		if (line[0] != ';' && equalPos != std::string::npos)
		{
			std::string fieldName = trim(line.substr(0, equalPos));
			if(iniSettings->containsField(fieldName))
			{
				LogManager::getInstance().log(FILE, NORMAL, INFO, filePath, "Duplicate field on .ini file. Only considering the 1st one.");
				continue;
			}

			std::string fieldValue = trim(line.substr(equalPos+1));
			iniSettings->addField(fieldName, fieldValue);
		}
	}
	inFile.close();
	return true;
}

std::string IniManager::trim(std::string &in)
{
	size_t first = in.find_first_not_of(' ');
	if (first == std::string::npos)
		return "";
	size_t last = in.find_last_not_of(' ');
	return in.substr(first, (last - first + 1));
}