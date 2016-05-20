#pragma once

#include <string>
#include <unordered_map>

#include "../io/log/LogManager.h"
#include "IFactory.h"

namespace Lag
{
	/*
	* Generic class for managers that store a type mapped by name. 
	*
	* Depends on (and manages) a IFactory that can create the type T. This allows GraphicsAPI specific objects (like shaders)
	* to only implement and override a simple Factory instead of deriving from a more complex Manager class.
	*/
	template<class T>
	class Manager
	{
	public:
		Manager(IFactory<T> *factory);
		virtual ~Manager();

		T* get(const std::string &name) const;

		virtual bool create(const std::string &name);

		virtual bool add(const std::string &name, T *obj);
		
		void remove(const std::string &name);

		IFactory<T> *getFactory() const { return factory; }

	protected:
		std::unordered_map<std::string, T*> objects;
		IFactory<T> *factory;
	};
	


	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class T>
	Manager<T>::Manager(IFactory<T> *factory) :
		factory(factory)
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
			"Manager", "Initialized successfully.");
	}

	template<class T>
	Manager<T>::~Manager()
	{
		for (auto &pair : objects)
		{
			delete pair.second;

			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
				"Manager", "Deleted " + pair.first);
		}

		delete factory;

		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
			"Manager", "Destroyed successfully.");
	}

	template<class T>
	bool Manager<T>::create(const std::string &name)
	{
		return add(name, factory->create());
	}

	template<class T>
	bool Manager<T>::add(const std::string &name, T *obj)
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

	template<class T>
	void Manager<T>::remove(const std::string &name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
				"Manager", "Removing object with name: " + name);
			delete it->second;
			objects.erase(name);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
				"Manager", "Trying to remove a non-existent object: " + name);
		}
	}

	template<class T>
	T* Manager<T>::get(const std::string &name) const
	{
		auto it = objects.find(name);
		if (it == objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_WARNING,
				"Manager", "Trying to get an unknown object: " + name);
			return nullptr;
		}

		return objects.at(name);
	}
}