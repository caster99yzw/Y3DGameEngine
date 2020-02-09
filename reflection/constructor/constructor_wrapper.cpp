#include "constructor_wrapper.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"
#include "reflection/parameter/parameter.h"

namespace reflection
{

bool ConstructorWrapperBase::Vaild() const
{
	return false;
}

Class ConstructorWrapperBase::DeclaringClass() const
{
	return Class();
}

common::array_view<Parameter> ConstructorWrapperBase::Parameters() const
{
	return common::array_view<Parameter>();
}

Variant ConstructorWrapperBase::Invoke() const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0, Argument &arg1) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0, Argument &arg1, Argument &arg2) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4, Argument &arg5) const
{
	return Variant();
}

Variant ConstructorWrapperBase::Invoke(std::vector<Argument> &args) const
{
	return Variant();
}

void ConstructorWrapperBase::Initialize()
{
	signature = GenerateSignatrueName();
}

std::string ConstructorWrapperBase::GenerateSignatrueName()
{
	return "myconstructor";
}

}

