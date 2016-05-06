#pragma once

//Guaranteed to be lazy initialized and that it will be destroyed correctly
//Copy constructor and assignement operator should not be implemented
//DO NOT define public constructors but implement the private one (and destructor)
#define GENERATE_SINGLETON(Type) public:\
static Type& getInstance() {\
	static Type instance;\
	return instance;\
}\
private:\
Type();\
~Type();\
Type(Type const& copy);\
Type& operator=(Type const& copy);