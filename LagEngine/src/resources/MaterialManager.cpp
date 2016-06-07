#include "MaterialManager.h"

#include "../renderer/Material.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MaterialManager::MaterialManager(const std::string &folder) :
	ResourceManager("MaterialManager", folder)
{
}

MaterialManager::~MaterialManager()
{
}

bool MaterialManager::create(const std::string &name, const std::string &file)
{
	return add(name, new Material(folder + '/' + file));
}

void MaterialManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "material")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");
		
		if (!name || !file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "MaterialManager",
				"A <material> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		create(name, file);
	}
}