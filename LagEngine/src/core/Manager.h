#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ManagedObject.h"
#include "Handle.h"
#include "../io/log/LogManager.h"
#include "../utils/Utils.h"

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

	/*
	* Generic class for Managers that store ManagedObjects mapped by a name.
	*/
	template<class K, class V>
	class Manager
	{
	public:
		Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder);
		virtual ~Manager();

		virtual void initialize() {}

		Handle<V> get(const K &name);
		inline std::unordered_map<K, V*> getAll() const { return objects; }
		
		inline IManagedObjectBuilder<K, V>& getBuilder() { return *builder; }

		void clear();
		void clearUnused();

		bool contains(const K &name) const;

	protected:
		bool load(V* object) const;
		void unload(V* object) const;

		//Deletes and removes the item from the map and returns a proper iterator to continue iteration
		//The iterator is assumed to be from the objects map
		typename std::unordered_map<K, V*>::iterator deleteEntry(typename std::unordered_map<K, V*>::iterator item);

		//Some Managers may want to have a fallback object to be returned when there is a problem getting the requested one
		Handle<V> defaultObject; //Handle stored here, avoiding the removal

		IManagedObjectBuilder<K, V> *builder;
		
		std::unordered_map<K, V*> objects;
		std::unordered_map<K, ControlBlock<V>*> controlBlocks; //used on Handles
		
		std::string logTag;
	};
#include "Manager.cpp"
}
