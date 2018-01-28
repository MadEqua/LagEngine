#pragma once

#include <string>
#include <unordered_map>

#include "ManagedObject.h"
#include "../io/log/LogManager.h"
#include "Utils.h"

namespace Lag
{
	template<class K, class V>
	class IManagedObjectBuilder
	{
	public:
		IManagedObjectBuilder() 
		{
			// Compile-time sanity check
			static_assert(std::is_base_of<ManagedObject, V>::value, "Creating a IManagedObjectBuilder of Vs not derived from ManagedObject");
		}

		virtual ~IManagedObjectBuilder() = default;
		virtual V* build(const K &name) const = 0;
	};


	template<class K, class V>
	class ManagedObjectListener : public IManagedObjectListener
	{
	public:
		ManagedObjectListener(Manager<K, V> &manager) : manager(manager) {}
		virtual ~ManagedObjectListener() = default;

		virtual void onZeroReferences(ManagedObject &object) const override
		{
			manager.remove(static_cast<V*>(&object));
		}

	private:
		Manager<K, V> &manager;
	};
	

	/*
	* Generic class for Managers that store ManagedObjects mapped by a name.
	*/
	template<class K, class V>
	class Manager
	{
	public:
		Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder);
		virtual ~Manager();

		V* get(const K &name);
		
		//When the parent is returned so will be the child (Useful for dependencies Ex: Texture -> Image)
		V* get(const K &name, ManagedObject &parent);

		inline std::unordered_map<K, V*> getAll() const { return objects; }
		
		inline IManagedObjectBuilder<K, V>& getBuilder() { return *builder; }

		void returnName(const K &name);
		void returnObject(V *object);

		bool contains(const K &name) const;

	protected:
		bool load(V* object) const;
		void unload(V* object) const;
		void unloadAll();

		void remove(V *object);

		template<class K, class V> friend class ManagedObjectListener;
		ManagedObjectListener<K, V> listener;

		IManagedObjectBuilder<K, V> *builder;
		std::unordered_map<K, V*> objects;
		std::string logTag;
	};



	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class K, class V>
	Manager<K, V>::Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder) :
		logTag(logTag),
		builder(builder),
		listener(*this)
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

		delete builder;

		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Destroyed successfully.");
	}

	template<class K, class V>
	V* Manager<K, V>::get(const K &name)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Getting object with name: " + toString(name));
		
		auto it = objects.find(name);
		V* object;

		if (it == objects.end())
		{
			object = builder->build(name);
			object->setName(toString(name));
			if (object == nullptr)
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
					logTag, "Error building Object " + toString(name));
				return nullptr;
			}

			object->setListener(listener);
			objects[name] = object;

			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Created object with name: " + toString(name));
		}
		else
		{
			object = it->second;
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Getting already existent object with name: " + toString(name));
		}

		if (load(object))
			return object;
		else
		{
			unload(object);
			return nullptr;
		}
	}

	template<class K, class V>
	V* Manager<K, V>::get(const K &name, ManagedObject &parent)
	{
		V *object = get(name);
		if(object != nullptr)
			parent.addDependency(object);
		return object;
	}

	template<class K, class V>
	void Manager<K, V>::returnName(const K &name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Returning object with name: " + toString(name));
			unload(it->second);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to return an unknown object: " + toString(name));
		}
	}

	template<class K, class V>
	void Manager<K, V>::returnObject(V *object)
	{
		if (object != nullptr)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Returning object: " + object->getName());
			unload(object);
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to return a null object.");
		}
	}

	template<class K, class V>
	void  Manager<K, V>::remove(V *object)
	{
		if (object != nullptr)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Removing object: " + object->getName());
			
			delete object;

			//TODO objects should store their own name, and avoid this
			for (auto it = objects.begin(); it != objects.end(); ++it)
				if (it->second == object)
				{
					objects.erase(it);
					return;
				}
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Trying to remove a null object.");
		}
	}

	template<class K, class V>
	bool Manager<K, V>::contains(const K &name) const
	{
		auto it = objects.find(name);
		return it != objects.end();
	}

	template<class K, class V>
	void Manager<K, V>::unloadAll()
	{
		for (auto &pair : objects)
			unload(pair.second);
	}

	template<class K, class V>
	bool Manager<K, V>::load(V* object) const
	{
		if (object->load())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Loaded object: " + object->getName());
			return true;
		}
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Failed to load object: " + object->getName());
			return false;
		}
	}

	template<class K, class V>
	void Manager<K, V>::unload(V* object) const
	{
		object->unload();
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Unloaded object: " + object->getName());
	}
}