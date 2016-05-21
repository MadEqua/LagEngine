#include "ResourceManager.h"

#include "Resource.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

ResourceManager::ResourceManager(const std::string &name) :
	Manager(name)
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