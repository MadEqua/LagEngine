#include "MeshManager.h"

#include "../renderer/Mesh.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::create(const std::string &name, const std::string &file)
{
	add(name, new Mesh(file));
}

void MeshManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "mesh")
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
			LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR, "MeshManager",
				"A <mesh> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_INFO, "MeshManager",
			"Mesh " + name + " has been declared from Resources file.");

		create(name, file);
	}
}