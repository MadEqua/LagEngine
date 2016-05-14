#include "MaterialManager.h"

#include "../renderer/Material.h"
#include "../io/log/LogManager.h"
#include "MaterialFactory.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
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
			LogManager::getInstance().log(FILE, NORMAL, ERROR, "MaterialManager",
				"A <material> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		LogManager::getInstance().log(FILE, NORMAL, INFO, "MaterialManager",
			"Material " + name + " has been declared from Resources file.");

		MaterialFactory factory(file);
		create(name, factory);
	}
}