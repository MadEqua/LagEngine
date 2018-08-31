#pragma once

#include "Types.h"
#include "ObserverPattern.h"

namespace Lag {

    class IObjectZeroReferencesListener {
    public:
        virtual ~IObjectZeroReferencesListener() = default;
        virtual void onZeroReferences() = 0;
    };


    template<class V>
    class ControlBlock {
    public:
        explicit ControlBlock(V *const pointer);

        inline V *const getPointer() const { return pointer; }

        inline void incrementRefCount() { refCount++; }
        void decrementRefCount();

        inline bool hasReferences() const { return refCount > 0; }
        inline uint32 getReferenceCount() const { return refCount; }

    LAG_GENERATE_SINGLE_OBSERVER_STORAGE(IObjectZeroReferencesListener)
    LAG_GENERATE_SINGLE_NOTIFY_METHOD(onZeroReferences, IObjectZeroReferencesListener, LAG_ARGS(), LAG_ARGS())

    private:
        V *const pointer;
        uint32 refCount;
    };


    /*
    * Handle to an object of type V.
    * Classes that depend on object V should hold a copy of this Handle.
    * The object will not be unloaded and destroyed while there are alive holders.
    * IMPORTANT: Do not hold Handles on classes that should not influence the lifecycle of the handled object.
    */
    template<class V>
    class Handle {
    public:
        Handle();
        explicit Handle(ControlBlock<V> &controlBlock);
        Handle(const Handle<V> &other);

        ~Handle();

        void invalidate();

        Handle<V> &operator=(const Handle<V> &other);
        inline V &operator*() const { return *controlBlock->getPointer(); }
        inline V *operator->() const { return controlBlock->getPointer(); }
        inline V *get() const { return controlBlock->getPointer(); }

        inline bool isValid() const { return controlBlock != nullptr && controlBlock->getPointer() != nullptr; }

    private:
        ControlBlock<V> *controlBlock;
    };


    template<class V>
    ControlBlock<V>::ControlBlock(V *const pointer) :
            refCount(0),
            pointer(pointer) {
    }

    template<class V>
    void ControlBlock<V>::decrementRefCount() {
        refCount--;
        if (refCount == 0)
            onZeroReferencesNotify();
    }


    template<class V>
    Handle<V>::Handle() : controlBlock(nullptr) {
    }

    template<class V>
    Handle<V>::Handle(ControlBlock<V> &controlBlock) :
            controlBlock(&controlBlock) {
        this->controlBlock->incrementRefCount();
    }

    template<class V>
    Handle<V>::Handle(const Handle<V> &other) {
        controlBlock = other.controlBlock;

        if (controlBlock != nullptr)
            controlBlock->incrementRefCount();
    }

    template<class V>
    Handle<V>::~Handle() {
        if (controlBlock != nullptr)
            controlBlock->decrementRefCount();
    }

    template<class V>
    Handle<V> &Handle<V>::operator=(const Handle<V> &other) {
        if(&other != this) {
            controlBlock = other.controlBlock;
            if(controlBlock != nullptr)
                controlBlock->incrementRefCount();
        }
        return *this;
    }

    template<class V>
    void Handle<V>::invalidate() {
        if (controlBlock != nullptr) {
            controlBlock->decrementRefCount();
            controlBlock = nullptr;
        }
    }
}

