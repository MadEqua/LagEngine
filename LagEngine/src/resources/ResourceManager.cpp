#include "ResourceManager.h"

#include "Resource.h"
#include "IResourceFactory.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

ResourceManager::ResourceManager()
{
	LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_INFO,
		"ResourceManager", "Initialized successfully.");
}

ResourceManager::~ResourceManager()
{
	for (auto &pair : resources)
	{
		pair.second->unload();
		delete pair.second;

		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_INFO,
			"ResourceManager", "Unloaded and deleted " + pair.first);
	}

	LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_INFO,
		"ResourceManager", "Destroyed successfully.");
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

void ResourceManager::create(const std::string &name, const IResourceFactory &factory)
{
	Resource *res = factory.create();
	add(name, res);
}

void ResourceManager::add(const std::string &name, Resource *res)
{
	bool ok = false;
	auto it = resources.find(name);
	if (it != resources.end())
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_WARNING, "ResourceManager",
			"Adding a resource with an already existing name: " + name + ". Only considering the first one added.");
	}
	else if (!res->load())
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR, "ResourceManager",
			"Failed to load Resource: " + name);
	}
	else
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_INFO, "ResourceManager",
			"Successfully loaded Resource: " + name);
		ok = true;
	}

	if (!ok)
	{
		res->unload();
		delete res;
	}
}

Resource* ResourceManager::get(const std::string &name) const
{
	std::string fullName = name;
	auto it = resources.find(fullName);
	if (it == resources.end())
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_WARNING, "ResourceManager",
			"Trying to get an unknown resource: " + name + ". Forgot to add it to the resources file? Did it fail to load? :)");
		return nullptr;
	}

	return resources.at(name);
}