#include "MeshManager.h"

#include "../renderer/Mesh.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MeshManager::MeshManager() :
	ResourceManager("MeshManager")
{
}

MeshManager::~MeshManager()
{
}

bool MeshManager::create(const std::string &name, const std::string &file)
{
	return add(name, new Mesh(file));
}

void MeshManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "mesh")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");
		
		if (!name || !file)
		{
			LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR, "MeshManager",
				"A <mesh> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		create(name, file);
	}
}