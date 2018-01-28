#pragma once

#include <vector>
#include <string>

namespace Lag
{
	class ManagedObject;
	
	class IManagedObjectListener
	{
	public:
		virtual ~IManagedObjectListener() = default;
		virtual void onZeroReferences(ManagedObject &object) const = 0;
	};
	
	/*
	* Object managed by a Manager, it has the concept of loading and unloading. 
	* It also maintains a reference count, when it reaches 0 the object will unload and notify the Manager.
	* The list of dependencies are other ManagedObjects that were loaded by this one.
	*/
	class ManagedObject
	{
	public:
		ManagedObject();
		
		inline const std::string& getName() const { return name; }
		inline void setName(const std::string &name) { this->name = name; }
		inline void setListener(IManagedObjectListener *listener) { this->listener = listener; }
		inline void addDependency(ManagedObject *object) { dependencies.push_back(object); }

	protected:
		virtual bool loadImplementation() = 0;
		virtual void unloadImplementation() = 0;

		int refCount;
		std::vector<ManagedObject*> dependencies;
		IManagedObjectListener *listener;
		std::string name;

	private:
		template<class K, class V> friend class Manager;
		bool load();
		void unload();

		void notifyZeroReferences();
	};
}
