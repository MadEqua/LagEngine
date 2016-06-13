#pragma once

#include <vector>
#include "../Types.h"

namespace Lag
{
	/*
	* Container for objects (pointers to) with an integer as a name. 
	* The container generates the names by itself.
	*/
	template<class T>
	class NamedContainer
	{
	public:
		NamedContainer();
		~NamedContainer();

		uint32 add(T *value);
		void set(uint32 name, T* value);
		void remove(uint32 name);
		T* get(uint32 name) const;

		bool contains(uint32 name) const;

		uint32 getNextName() const;
		inline uint32 getSize() const { return data.size(); }

	private:
		std::vector<T*> data;
	};



	template<class T>
	NamedContainer<T>::NamedContainer()
	{
	}
	
	template<class T>
	NamedContainer<T>::~NamedContainer()
	{
		for (T* value : data)
			if(value != nullptr)
				delete value;
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