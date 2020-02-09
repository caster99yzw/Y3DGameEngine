#pragma once
#include "method_wrapper.h"

namespace reflection
{

class Variant;
class Class;
class Argument;

class Method
{
public:
	Method() = default;
	Method(MethodWrapperBase const *inWrapper)
		: wrapper(inWrapper) {}

	bool Valid() const;
	std::string const &Name() const;
	common::array_view<Parameter> Parameters() const;

	Variant Invoke(Variant &obj) const;
	Variant Invoke(Variant &obj, Argument arg0) const;
	Variant Invoke(Variant &obj, Argument arg0, Argument arg1) const;
	Variant Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2) const;
	Variant Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3) const;
	Variant Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4) const;
	Variant Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4, Argument arg5) const;
	Variant Invoke(Variant &obj, std::vector<Argument> args) const;

private:
	MethodWrapperBase const *wrapper = nullptr;
};

}




