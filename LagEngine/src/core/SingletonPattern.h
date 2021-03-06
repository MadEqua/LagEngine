#pragma once

//Guaranteed to be lazy initialized and that it will be destroyed correctly
//Copy constructor and assignement operator should not be implemented
//DO NOT define public constructors but implement the private one (and destructor)
#define LAG_GENERATE_SINGLETON(Type) public:\
static Type& getInstance() {\
	static Type instance;\
	return instance;\
}\
private:\
Type();\
~Type();\
Type(const Type &copy) = delete;\
Type& operator=(const Type &copy) = delete;
