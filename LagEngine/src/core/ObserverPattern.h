#pragma once

#include <forward_list>

/*
* Facilitator for implementing the Subject(aka Notifier) from the Observer Pattern on any class.
*
* IMPORTANT: Do not rely on this for deletion/dealocation of the registered Observers (aka Listeners). 
* Always cleanup the Listener interfaces by yourself.
*/

//Add methods and storage allowing a class to be used as a Subject/Notifier of Observers/Listeners.
#define LAG_GENERATE_OBSERVER_STORAGE(Type)\
protected:\
std::forward_list<Type*> Type##observers;\
public:\
void registerObserver(Type &observer) { Type##observers.push_front(&observer); }\
void unregisterObserver(Type &observer) { Type##observers.remove(&observer); }

#define LAG_ARGS(...) __VA_ARGS__

//Add a notifier method for Observers of <Type> with arbitrary name and arguments. This method notifies all registered Observers.
//The method will have the same name as the one present on the Observer interface.
//Example use: GENERATE_NOTIFY_METHOD(ISomeListener, onFooEvent, LAG_ARGS(int arg1, float arg2), LAG_ARGS(arg1, arg2))

#define LAG_DECLARE_NOTIFY_METHOD(Name, MethodArguments)\
protected:\
void Name##Notify(MethodArguments);

#define LAG_DEFINE_NOTIFY_METHOD(ClassName, Name, Type, MethodArguments, CallbackArgs)\
void ClassName##::Name##Notify(MethodArguments) { for (Type *obs : Type##observers) obs->Name(CallbackArgs); }