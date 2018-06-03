#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ManagedObject.h"
#include "Handle.h"
#include "LogManager.h"
#include "Utils.h"

#include "Root.h"

namespace Lag {
    template<class K, class V>
    class IManagedObjectBuilder {
    public:
        IManagedObjectBuilder() {
            // Compile-time sanity check
            static_assert(std::is_base_of<ManagedObject, V>::value,
                          "Creating a IManagedObjectBuilder of Vs not derived from ManagedObject");
        }

        virtual ~IManagedObjectBuilder() = default;

        virtual V *build(const K &name) const = 0;
    };

    /*
    * Generic class for Managers that store ManagedObjects mapped by a name.
    */
    template<class K, class V>
    class Manager : public IObjectZeroReferencesListener {
    public:
        Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder);
        virtual ~Manager();

        virtual void initializeFallbackObject() {}

        Handle<V> get(const K &name);
        inline std::unordered_map<K, V *> getAll() const { return objects; }
        inline IManagedObjectBuilder<K, V> &getBuilder() { return *builder; }

        void clear();
        void clearUnused();

        bool reload(const K &name);
        bool reloadAll();

        bool contains(const K &name) const;
        uint32 getReferenceCount(const K &name) const;
        void onZeroReferences() override;

    protected:
        bool load(V *object) const;

        void unload(V *object) const;

        //Deletes and removes the item from the map and returns a proper iterator to continue iteration
        //The iterator is assumed to be from the objects map
        typename std::unordered_map<K, V *>::iterator deleteEntry(typename std::unordered_map<K, V *>::iterator item);

        //Some Managers may want to have a fallback object to be returned when there is a problem getting the requested one
        Handle<V> fallbackObject; //Handle stored here, avoiding the removal

        IManagedObjectBuilder<K, V> *builder;

        std::unordered_map<K, V *> objects;
        std::unordered_map<K, ControlBlock<V> *> controlBlocks; //used on Handles

        std::string logTag;
    };


    template<class K, class V>
    Manager<K, V>::Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder) :
            logTag(logTag),
            builder(builder) {
        // Compile-time sanity check
        static_assert(std::is_base_of<ManagedObject, V>::value,
                      "Creating a Manager of Values not derived from ManagedObject");

        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Initialized successfully.");
    }

    template<class K, class V>
    Manager<K, V>::~Manager() {
        clear();

        delete builder;

        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Destroyed successfully.");
    }

    template<class K, class V>
    Handle<V> Manager<K, V>::get(const K &name) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Getting object with name: " + Utils::toString(name));

        auto it = objects.find(name);
        V *object;
        ControlBlock<V> *controlBlock;

        if (it == objects.end()) {
            object = builder->build(name);
            if (object == nullptr) {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                              logTag, "Error building Object " + Utils::toString(name));
                return fallbackObject;
            }

            object->setName(Utils::toString(name));
            objects[name] = object;

            controlBlock = new ControlBlock<V>(object);
            controlBlocks[name] = controlBlock;

            LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                          logTag, "Created object with name: " + Utils::toString(name));
        }
        else {
            object = it->second;
            controlBlock = controlBlocks[name];
            LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                          logTag,
                                          "Getting already existent object with name: " + Utils::toString(name));
        }

        if (load(object)) {
            Handle<V> handle(*controlBlock);
            controlBlock->registerObserver(*this);
            return handle;
        }
        else {
            deleteEntry(objects.find(name));
            return fallbackObject;
        }
    }

    template<class K, class V>
    bool Manager<K, V>::contains(const K &name) const {
        auto it = objects.find(name);
        return it != objects.end();
    }

    template<class K, class V>
    uint32 Manager<K, V>::getReferenceCount(const K &name) const {
        auto it = controlBlocks.find(name);
        if (it == controlBlocks.end())
            return 0;
        else
            return it->second->getReferenceCount();
    }

    template<class K, class V>
    void Manager<K, V>::clear() {
        for (auto &pair : objects) {
            unload(pair.second);
            delete pair.second;
        }

        for (auto &pair : controlBlocks) {
            delete pair.second;
        }

        objects.clear();
        controlBlocks.clear();

        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Cleared all objects successfully.");
    }

    template<class K, class V>
    void Manager<K, V>::clearUnused() {
        int count = 0;
        for (auto it = objects.begin(); it != objects.end();) {
            auto &objectPair = *it;
            ControlBlock<V> *controlBlock = controlBlocks[objectPair.first];

            if (!controlBlock->hasReferences()) {
                LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                              logTag, "Clearing unused object: " + objectPair.second->getName() + ".");

                it = deleteEntry(it);
                ++count;
            }
            else
                ++it;
        }

        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Cleared " + std::to_string(count) + " unused objects.");
    }

    template<class K, class V>
    bool Manager<K, V>::reload(const K &name) {
        auto it = objects.find(name);
        V *object;

        if (it == objects.end()) {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                          logTag, "Trying to reload unknown object: " + name);
            return false;
        }
        else {
            object = it->second;
        }

        unload(object);
        return load(object);
    }

    template<class K, class V>
    bool Manager<K, V>::reloadAll() {
        for (auto &pair : objects) {
            V *object = pair.second;
            unload(object);
            if (!load(object))
                return false;
        }
        return true;
    }

    template<class K, class V>
    bool Manager<K, V>::load(V *object) const {
        if (object->load()) {
            LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                          logTag, "Loaded object: " + object->getName());
            return true;
        }
        else {
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                          logTag, "Failed to load object: " + object->getName());
            return false;
        }
    }

    template<class K, class V>
    void Manager<K, V>::unload(V *object) const {
        object->unload();
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      logTag, "Unloaded object: " + object->getName());
    }

    template<class K, class V>
    typename std::unordered_map<K, V *>::iterator
    Manager<K, V>::deleteEntry(typename std::unordered_map<K, V *>::iterator item) {
        V *object = item->second;
        ControlBlock<V> *controlBlock = controlBlocks.at(item->first);

        unload(object); //Unload ManagedObject
        delete object; //Deallocate ManagedObject heap memory
        delete controlBlock; //Deallocate ControlBlock

        //Remove from maps
        controlBlocks.erase(item->first);
        return objects.erase(item);
    }

    template<class K, class V>
    void Manager<K, V>::onZeroReferences() {
        clearUnused();
    }
}



