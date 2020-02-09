#include "method_wrapper.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"
#include "reflection/parameter/parameter.h"

namespace reflection
{


bool MethodWrapperBase::Vaild() const
{
	return false;
}

std::string const &MethodWrapperBase::Name() const
{
	static std::string invalid = "";
	return invalid;
}

Class MethodWrapperBase::DeclaringClass() const
{
	return Class();
}

common::array_view<Parameter> MethodWrapperBase::Parameters() const
{
	return common::array_view<Parameter>();
}

Variant MethodWrapperBase::Invoke(Variant &obj) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0, Argument &arg1) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4, Argument &arg5) const
{
	return Variant();
}

Variant MethodWrapperBase::Invoke(Variant &obj, std::vector<Argument> &args) const
{
	return Variant();
}

void MethodWrapperBase::Initialize()
{
	GenerateSignatrueName();
}

std::string MethodWrapperBase::GenerateSignatrueName()
{
	return "mymethod";
}

}

