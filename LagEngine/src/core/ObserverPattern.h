#pragma once

#include <forward_list>

/*
* Facilitator for implementing the Subject(aka Notifier or Observable) from the Observer Pattern on any class.
*
* IMPORTANT: Do not rely on this for deletion/dealocation of the registered Observers (aka Listeners). 
* Always cleanup the Listener interfaces by yourself (better yet: use auto-memory management).
*/


//Add methods and storage allowing a class to be used as a Subject/Notifier of Observers/Listeners.
#define LAG_GENERATE_OBSERVER_STORAGE(Type)\
protected:\
std::forward_list<Type*> Type##Observers;\
public:\
void registerObserver(Type &observer) { Type##Observers.push_front(&observer); }\
void unregisterObserver(Type &observer) { Type##Observers.remove(&observer); }

#define LAG_ARGS(...) __VA_ARGS__

//Add a notifier method for Observers of <Type> with arbitrary name and arguments. This method notifies all registered Observers.
//The method will have the same name as the one present on the Observer interface.
//Example use: GENERATE_NOTIFY_METHOD(onFooEvent, ISomeListener, LAG_ARGS(int arg1, float arg2), LAG_ARGS(arg1, arg2))
#define LAG_GENERATE_NOTIFY_METHOD(Name, Type, MethodArguments, CallbackArgs)\
protected:\
inline void Name##Notify(MethodArguments) { for (Type *obs : Type##Observers) obs->Name(CallbackArgs); }


//
//Same as above, but supporting only a single observer.
//
#define LAG_GENERATE_SINGLE_OBSERVER_STORAGE(Type)\
protected:\
Type* Type##Observer = nullptr;\
public:\
void registerObserver(Type &observer) { Type##Observer = &observer; }\
void unregisterObserver(Type &observer) { Type##Observer = nullptr; }

#define LAG_GENERATE_SINGLE_NOTIFY_METHOD(Name, Type, MethodArguments, CallbackArgs)\
protected:\
inline void Name##Notify(MethodArguments) { if(Type##Observer != nullptr) Type##Observer->Name(CallbackArgs); }