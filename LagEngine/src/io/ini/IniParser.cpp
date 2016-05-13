#include "IniParser.h"
#include <fstream>
#include "IniSettings.h"
#include "../log/LogManager.h"

using namespace Lag;

IniParser::IniParser()
{
}

IniParser::~IniParser()
{
}

bool IniParser::parse(const std::string &filePath)
{	
	std::ifstream inFile(filePath, std::ios::in);
	if (!inFile.is_open())
	{
		LogManager::getInstance().log(FILE, NORMAL, ERROR, "IniParser", "Error opening .ini file: " + filePath);
		return false;
	}
		
	std::string line;
	while (std::getline(inFile, line))
	{
		line = trim(line);
		size_t equalPos = line.find('=');
		if (line[0] != ';' && equalPos != std::string::npos)
		{
			std::string fieldName = trim(line.substr(0, equalPos));
			if(out.containsField(fieldName))
			{
				LogManager::getInstance().log(FILE, NORMAL, WARNING, "IniParser", 
					"Duplicate field on .ini file: " + filePath + ". Only considering the first appearance.");
				continue;
			}

			std::string fieldValue = trim(line.substr(equalPos+1));
			out.addField(fieldName, fieldValue);
		}
	}
	inFile.close();
	return true;
}

std::string IniParser::trim(std::string &in) const
{
	size_t first = in.find_first_not_of(' ');
	if (first == std::string::npos)
		return "";
	size_t last = in.find_last_not_of(' ');
	return in.substr(first, (last - first + 1));
}