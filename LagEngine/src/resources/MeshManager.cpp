#include "MeshManager.h"

#include "../io/log/LogManager.h"
#include "MeshFactory.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
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
			LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::ERROR, "MeshManager",
				"A <mesh> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::INFO, "MeshManager",
			"Mesh " + name + " has been declared from Resources file.");

		MeshFactory factory(file);
		create(name, factory);
	}
}