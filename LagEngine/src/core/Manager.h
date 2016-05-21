#pragma once

#include <string>
#include <unordered_map>

#include "../io/log/LogManager.h"

namespace Lag
{
	/*
	* Generic class for managers that store a type mapped by a name.
	*/
	template<class K, class V>
	class Manager
	{
	public:
		Manager();
		virtual ~Manager();

		V* get(const K &name) const;
		virtual bool add(const K &name, V *obj);
		void remove(const K &name);

	protected:
		std::unordered_map<K, V*> objects;
	};
	


	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class K, class V>
	Manager<K, V>::Manager()
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
			"Manager", "Initialized successfully.");
	}

	template<class K, class V>
	Manager<K, V>::~Manager()
	{
		for (auto &pair : objects)
		{
			delete pair.second;

			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
				"Manager", "Deleted " + pair.first);
		}

		//delete factory;

		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
			"Manager", "Destroyed successfully.");
	}

	template<class K, class V>
	bool Manager<K, V>::add(const K &name, V *obj)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
				"Manager", "Trying to add an object with an already existing name: " + name + ". Only considering the first one added.");
			delete obj;
			return false;
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
				"Manager", "Successfully added object: " + name);

			objects[name] = obj;
			return true;
		}
	}

	template<class K, class V>
	void Manager<K, V>::remove(const K &name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
				"Manager", "Removing object with name: " + name);
			delete it->second;
			objects.erase(it);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
				"Manager", "Trying to remove a non-existent object: " + name);
		}
	}

	template<class K, class V>
	V* Manager<K,V>::get(const K &name) const
	{
		auto it = objects.find(name);
		if (it == objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
				"Manager", "Trying to get an unknown object: " + name);
			return nullptr;
		}

		return it->second;
	}
}