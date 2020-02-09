#include "property_wrapper.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"

namespace reflection
{

bool PropertyWrapperBase::Vaild() const
{
	return false;
}

std::string const & PropertyWrapperBase::Name() const
{
	static std::string invalid = "";
	return invalid;
}

Class PropertyWrapperBase::DeclaringClass() const
{
	return Class();
}

bool PropertyWrapperBase::SetValue(Variant &obj, Argument &arg) const
{
	return false;
}

Variant PropertyWrapperBase::Value(Variant &obj) const
{
	return Variant();
}

void PropertyWrapperBase::Initialize()
{
}

}

