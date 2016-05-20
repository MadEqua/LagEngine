#include "ResourceManager.h"

#include "Resource.h"
#include "IResourceFactory.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

ResourceManager::ResourceManager(IResourceFactory *factory) :
	Manager(factory)
{
}

ResourceManager::~ResourceManager()
{
	for (auto &pair : objects)
	{
		pair.second->unload();
	}
}

void ResourceManager::initalizeFromResourcesFile(const TiXmlElement &resourceFileRoot)
{
	for (const TiXmlElement* elem = resourceFileRoot.FirstChildElement();
		elem != NULL;
		elem = elem->NextSiblingElement())
	{
		parseResourceDescription(*elem);	
	}
}

bool ResourceManager::add(const std::string &name, Resource *res)
{
	if (Manager::add(name, res))
	{
		if (!res->load())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR,
				"ResourceManager", "Failed to load Resource: " + name);

			remove(name);
			return false;
		}
		return true;
	}
	return false;
}