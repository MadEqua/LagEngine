#pragma once

#include <vector>
#include "../Types.h"

namespace Lag
{
	/*
	* Container for objects (pointers to) with an integer as a name. 
	* The container generates the names by itself.
	* Useful for storing objects that need a name, but not an user-friendly one (not exposed to the application).
	*
	* TODO: it would be nice to have an iterator for this container
	*/
	template<class T>
	class NamedContainer
	{
	public:
		~NamedContainer();

		uint32 add(T *value);
		void set(uint32 name, T* value);
		void remove(uint32 name);
		T* get(uint32 name) const;

		void clear();

		bool contains(uint32 name) const;

		uint32 getNextName() const;
		inline uint32 getSize() const { return data.size(); }
		inline bool isEmpty() const { return data.empty(); }

	private:
		std::vector<T*> data;
	};



	template<class T>
	NamedContainer<T>::~NamedContainer()
	{
		clear();
	}

	template<class T>
	uint32 NamedContainer<T>::add(T *value)
	{
		uint32 index = getNextName();
		
		if (index < data.size())
			data[index] = value;
		else
			data.push_back(value);

		return index;
	}

	template<class T>
	void NamedContainer<T>::set(uint32 name, T* value)
	{
		if (name < data.size())
			data[name] = value;
	}

	template<class T>
	void NamedContainer<T>::remove(uint32 name)
	{
		if (name < data.size())
		{
			delete data[name];
			data[name] = nullptr;
		}
	}

	template<class T>
	T* NamedContainer<T>::get(uint32 name) const
	{
		if (name < data.size())
			return data[name];
		else
			return nullptr;
	}

	template<class T>
	void NamedContainer<T>::clear()
	{
		for (T* value : data)
			if (value != nullptr)
				delete value;

		data.clear();
	}

	template<class T>
	bool NamedContainer<T>::contains(uint32 name) const
	{
		return name < data.size() && data[name] != nullptr;
	}

	template<class T>
	uint32 NamedContainer<T>::getNextName() const
	{
		for (uint32 i = 0; i < data.size(); ++i)
			if (data[i] == nullptr)
				return i;

		return data.size();
	}
}