#pragma once

#include <vector>
#include <memory>

#include "Types.h"

namespace Lag {
    /*
    * Container for objects (pointers to) with an integer as a name.
    * The container generates the names by itself.
    * Useful for storing objects that need a name, but not an user-friendly one (not exposed to the application).
    *
    * TODO: it would be nice to have an iterator for this container
    */
    template<class T>
    class NamedContainer {
    public:
        uint32 add(T *value);
        void set(uint32 name, T *value);
        void remove(uint32 name);
        T* get(uint32 name) const;

        void clear();

        bool contains(uint32 name) const;

        uint32 getSize() const;
        bool isEmpty() const;

        uint32 getNextName() const;

    private:
        std::vector<std::unique_ptr<T>> data;
    };

    template<class T>
    uint32 NamedContainer<T>::add(T *value) {
        uint32 index = getNextName();

        if (index < data.size())
            set(index, value);
        else
            data.push_back(std::unique_ptr<T>(value));

        return index;
    }

    template<class T>
    void NamedContainer<T>::set(uint32 name, T *value) {
        if (name < data.size()) {
            data[name] = std::unique_ptr<T>(value);
        }
    }

    template<class T>
    void NamedContainer<T>::remove(uint32 name) {
        if (name < data.size()) {
            data[name].reset();
        }
    }

    template<class T>
    T *NamedContainer<T>::get(uint32 name) const {
        if (name < data.size())
            return data[name].get();
        else
            return nullptr;
    }

    template<class T>
    void NamedContainer<T>::clear() {
        data.clear();
    }

    template<class T>
    bool NamedContainer<T>::contains(uint32 name) const {
        return name < data.size() && data[name];
    }

    template<class T>
    uint32 NamedContainer<T>::getSize() const {
        uint32 count = 0;
        for(auto &up : data) {
            if(up) count++;
        }
        return count;
    }

    template<class T>
    bool NamedContainer<T>::isEmpty() const {
        return getSize() == 0;
    }

    template<class T>
    uint32 NamedContainer<T>::getNextName() const {
        for (uint32 i = 0; i < data.size(); ++i)
            if (!data[i])
                return i;

        return static_cast<uint32>(data.size());
    }
}