#include "MeshManager.h"

#include "../renderer/Mesh.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MeshManager::MeshManager(const std::string &folder) :
	XmlResourceManager("MeshManager", folder)
{
}

bool MeshManager::create(const std::string &name, const std::string &file)
{
	return add(name, new Mesh(folder + '/' + file));
}

void MeshManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "mesh")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");
		
		if (!name || !file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "MeshManager",
				"A <mesh> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		create(name, file);
	}
}