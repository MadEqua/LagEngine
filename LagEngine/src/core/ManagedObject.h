#pragma once

namespace Lag
{
	class ManagedObject
	{
	public:
		ManagedObject();
		virtual ~ManagedObject();
		
		bool load();
		void unload();

		inline bool isLoaded() const { return loaded; }

	protected:
		virtual bool loadImplementation() = 0;
		virtual void unloadImplementation() = 0;

		bool loaded;
	};
}
