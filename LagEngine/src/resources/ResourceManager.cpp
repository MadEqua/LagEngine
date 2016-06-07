#include "ResourceManager.h"

#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

ResourceManager::ResourceManager(const std::string &name, const std::string &folder) :
	Manager(name),
	folder(folder)
{
}

ResourceManager::ResourceManager(const std::string &name) :
	Manager(name),
	folder("")
{
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::parseResourceFile(const TiXmlElement &resourceFileRoot)
{
	for (const TiXmlElement* elem = resourceFileRoot.FirstChildElement();
		elem != NULL;
		elem = elem->NextSiblingElement())
	{
		parseResourceDescription(*elem);
	}
}