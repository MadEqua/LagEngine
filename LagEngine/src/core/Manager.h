#pragma once

#include <string>
#include <unordered_map>

#include "ManagedObject.h"
#include "../io/log/LogManager.h"
#include "Utils.h"

namespace Lag
{
	/*
	* Generic class for Managers that store ManagedObjects mapped by a name.
	*/
	template<class K, class V>
	class Manager
	{
	public:
		explicit Manager(const std::string &logTag);
		virtual ~Manager();

		V* get(const K &name) const;
		bool contains(const K &name) const;
		
		bool add(const K &name, V *obj);
		//bool addAndLoad(const K &name, V *obj);
		void remove(const K &name);

		bool load(const K &name) const;
		void unload(const K &name) const;

		void loadAll();
		void unloadAll();

	protected:

		//Will contain all added objects. At any point in time some will be loaded and some unloaded.
		std::unordered_map<K, V*> objects;
		std::string logTag;
	};



	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class K, class V>
	Manager<K, V>::Manager(const std::string &logTag) :
		logTag(logTag)
	{
		// Compile-time sanity check
		static_assert(std::is_base_of<ManagedObject, V>::value, "Creating a Manager of Values not derived from ManagedObject");
		
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Initialized successfully.");
	}

	template<class K, class V>
	Manager<K, V>::~Manager()
	{
		unloadAll();

		for (auto &pair : objects)
			delete pair.second;

		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Destroyed successfully.");
	}

	template<class K, class V>
	bool Manager<K, V>::add(const K &name, V *obj)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to add an object with an already existing name: " + toString(name) + ". Only considering the first one added.");
			delete obj;
			return false;
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Successfully added object: " + toString(name));

			objects[name] = obj;
			return true;
		}
	}

	/*template<class K, class V>
	bool Manager<K, V>::addAndLoad(const K &name, V *obj)
	{
		if (add(name, obj))
			return load(name);

		return false;
	}*/

	template<class K, class V>
	bool Manager<K, V>::load(const K &name) const
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			if (it->second->load())
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Loaded object: " + toString(name));
				return true;
			}
			else
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Failed to load object: " + toString(name));
				return false;
			}
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to load an unknown object: " + toString(name) + ".");
			return false;
		}
	}

	template<class K, class V>
	void Manager<K, V>::unload(const K &name) const
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			it->second->unload();
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Unloaded object: " + toString(name));
		}
		else
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to unload an unknown object: " + toString(name) + ".");
	}

	template<class K, class V>
	void Manager<K, V>::remove(const K &name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Removing object with name: " + toString(name));
			
			it->second->unload();
			delete it->second;
			objects.erase(it);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to remove a non-existent object: " + toString(name));
		}
	}

	template<class K, class V>
	V* Manager<K, V>::get(const K &name) const
	{
		auto it = objects.find(name);
		if (it == objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to get an unknown object: " + toString(name));
			return nullptr;
		}

		return it->second;
	}

	template<class K, class V>
	bool Manager<K, V>::contains(const K &name) const
	{
		auto it = objects.find(name);
		return it != objects.end();
	}

	template<class K, class V>
	void Manager<K, V>::loadAll()
	{
		for (auto &pair : objects)
			load(pair.first);
	}

	template<class K, class V>
	void Manager<K, V>::unloadAll()
	{
		for (auto &pair : objects)
			unload(pair.first);
	}
}