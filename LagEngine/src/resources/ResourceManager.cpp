#include "ResourceManager.h"

#include "Resource.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

ResourceManager::ResourceManager()
{
	LogManager::getInstance().log(FILE, NORMAL, INFO, "ResourceManager", "Initialized successfully.");
}

ResourceManager::~ResourceManager()
{
	for (auto &pair : resources)
	{
		pair.second->unload();
		delete pair.second;

		LogManager::getInstance().log(FILE, NORMAL, INFO, "ResourceManager", "Unloaded and deleted " + pair.first);
	}

	LogManager::getInstance().log(FILE, NORMAL, INFO, "ResourceManager", "Destroyed successfully.");
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

void ResourceManager::add(const std::string &name, Resource *res)
{
	auto it = resources.find(name);
	if (it != resources.end())
	{
		LogManager::getInstance().log(FILE, NORMAL, INFO, "ResourceManager", 
			"Adding a resource with an already existing name: " + name + ". Only considering the first one added.");

		res->unload();
		delete res;
	}
	else
	{
		if (!res->load())
		{
			LogManager::getInstance().log(FILE, NORMAL, ERROR, "ResourceManager",
				"Failed to load Resource: " + name);

			res->unload();
			delete res;
		}
		else
		{
			LogManager::getInstance().log(FILE, NORMAL, INFO, "ResourceManager",
				"Successfully loaded Resource: " + name);
		}
	}
}

Resource* ResourceManager::get(const std::string &name) const
{
	std::string fullName = name;
	auto it = resources.find(fullName);
	if (it == resources.end())
	{
		LogManager::getInstance().log(FILE, NORMAL, ERROR, "ResourceManager",
			"Trying to get an unknown resource: " + name + ". Forgot to add it to the resources file? :)");
		return nullptr;
	}

	return resources.at(name);
}