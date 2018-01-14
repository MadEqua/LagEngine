#pragma once

namespace Lag
{
	class ManagedObject
	{
	public:
		ManagedObject();
		virtual ~ManagedObject() = default;

		inline bool isLoaded() const { return loaded; }

	protected:
		template<class K, class V> friend class Manager;
		bool load();
		void unload();

		virtual bool loadImplementation() = 0;
		virtual void unloadImplementation() = 0;

		bool loaded;
	};
}
