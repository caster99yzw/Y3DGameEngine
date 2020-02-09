#pragma once
#include "constructor_wrapper.h"

namespace reflection
{

class Variant;
class Class;
class Argument;

class Constructor
{
public:
	Constructor() = default;
	Constructor(ConstructorWrapperBase const *inWrapper)
		: wrapper(inWrapper) {}

	bool Vaild() const;
	Class DeclaringClass() const;
	common::array_view<Parameter> Parameters() const;

	Variant Invoke() const;
	Variant Invoke(Argument arg0) const;
	Variant Invoke(Argument arg0, Argument arg1) const;
	Variant Invoke(Argument arg0, Argument arg1, Argument arg2) const;
	Variant Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3) const;
	Variant Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4) const;
	Variant Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4, Argument arg5) const;
	Variant Invoke(std::vector<Argument> args) const;

private:
	ConstructorWrapperBase const *wrapper = nullptr;
};

}



