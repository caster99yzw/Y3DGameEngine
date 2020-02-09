#pragma once
#include "property_wrapper.h"

namespace reflection
{

class Variant;
class Class;
class Argument;

class Property
{
public:
	Property() = default;
	Property(PropertyWrapperBase const *inWrapper)
		: wrapper(inWrapper) {}

	bool Vaild() const;
	std::string const &Name() const;
	Class DeclaringClass() const;
	
	bool SetValue(Variant &obj, Argument arg);
	Variant Value(Variant &obj);

private:
	PropertyWrapperBase const *wrapper = nullptr;
};

}




