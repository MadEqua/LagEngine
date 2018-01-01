#pragma once

#include <string>
#include <unordered_map>

#include "ManagedObject.h"
#include "../io/log/LogManager.h"

namespace Lag
{
	/*
	* Generic class for managers that store ManagedObjects mapped by a name.
	* Loads and unloads can be decoupled from adds.
	*/
	template<class K>
	class Manager
	{
	public:
		explicit Manager(const std::string &logTag);
		virtual ~Manager();

		ManagedObject* get(const K &name) const;
		bool contains(const K &name) const;
		
		virtual bool add(const K &name, ManagedObject *obj);
		bool addAndLoad(const K &name, ManagedObject *obj);
		void remove(const K &name);

		void loadAll();
		void unloadAll();

	protected:
		std::unordered_map<K, ManagedObject*> objects;
		std::string logTag;
	};



	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class K>
	Manager<K>::Manager(const std::string &logTag) :
		logTag(logTag)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Initialized successfully.");
	}

	template<class K>
	Manager<K>::~Manager()
	{
		unloadAll();

		for (auto &pair : objects)
		{
			delete pair.second;

			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Deleted " + pair.first);
		}

		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Destroyed successfully.");
	}

	template<class K>
	bool Manager<K>::add(const K &name, ManagedObject *obj)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to add an object with an already existing name: " + name + ". Only considering the first one added.");
			delete obj;
			return false;
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Successfully added object: " + name);

			objects[name] = obj;
			return true;
		}
	}

	template<class K>
	virtual bool Manager<K>::addAndLoad(const K &name, ManagedObject *obj)
	{
		if (add(name, obj))
		{
			if (obj->load())
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Loaded Resource: " + pair.first);
				return true;
			}
			else
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Failed to load Resource: " + pair.first);
				return false;
			}
		}
		return false;
	}

	template<class K>
	void Manager<K>::remove(const K &name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Removing object with name: " + name);
			delete it->second;
			objects.erase(it);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to remove a non-existent object: " + name);
		}
	}

	template<class K>
	ManagedObject* Manager<K>::get(const K &name) const
	{
		auto it = objects.find(name);
		if (it == objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to get an unknown object: " + name);
			return nullptr;
		}

		return it->second;
	}

	template<class K>
	bool Manager<K>::contains(const K &name) const
	{
		auto it = objects.find(name);
		return it != objects.end();
	}

	template<class K>
	void Manager<K>::loadAll()
	{
		for (auto &pair : objects)
		{
			if (pair.second->load())
				LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Loaded Resource: " + pair.first);
			else
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Failed to load Resource: " + pair.first);
		}
	}

	template<class K>
	void Manager<K>::unloadAll()
	{
		for (auto &pair : objects)
		{
			if (pair.second->isLoaded())
			{
				pair.second->unload();
				LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Unloaded Resource: " + pair.first);
			}
		}
	}
}