#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ManagedObject.h"
#include "Handle.h"
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


	/*
	* Generic class for Managers that store ManagedObjects mapped by a name.
	*/
	template<class K, class V>
	class Manager
	{
	public:
		Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder);
		virtual ~Manager();

		Handle<V> get(const K &name);
		inline std::unordered_map<K, V*> getAll() const { return objects; }
		
		inline IManagedObjectBuilder<K, V>& getBuilder() { return *builder; }

		void clear();
		void clearUnused();

		bool contains(const K &name) const;

	protected:
		bool load(V* object) const;
		void unload(V* object) const;

		IManagedObjectBuilder<K, V> *builder;
		
		std::unordered_map<K, V*> objects;
		std::unordered_map<K, ControlBlock<V>*> controlBlocks; //used on Handles
		
		std::string logTag;
	};

#include "Manager.cpp"
}
