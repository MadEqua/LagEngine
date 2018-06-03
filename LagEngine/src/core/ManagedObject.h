#pragma once

#include <string>

namespace Lag {

    /*
    * Object managed by a Manager, it has the concept of loading and unloading.
    */
    class ManagedObject {
    public:
        ManagedObject();
        virtual ~ManagedObject() = default;

        inline const std::string &getName() const { return name; }
        inline void setName(const std::string &name) { this->name = name; }

    protected:
        virtual bool loadImplementation() = 0;
        virtual void unloadImplementation() = 0;

        std::string name;

    private:
        template<class K, class V> friend
        class Manager;

        bool load();
        void unload();

        bool isLoaded;
    };
}
