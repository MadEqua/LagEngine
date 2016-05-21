#include "MaterialManager.h"

#include "../renderer/Material.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::create(const std::string &name, const std::string &file)
{
	add(name, new Material(file));
}

void MaterialManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "material")
	{
		std::string name, file;
		
		for (const TiXmlElement* child = element.FirstChildElement();
			child != NULL;
			child = child->NextSiblingElement())
		{
			if (child->ValueStr() == "name")
				name = child->GetText();
			else if (child->ValueStr() == "file")
				file = child->GetText();
		}

		if (name.empty() || file.empty())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "MaterialManager",
				"A <material> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO, "MaterialManager",
			"Material " + name + " has been declared from Resources file.");

		create(name, file);
	}
}