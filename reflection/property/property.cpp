#include "property.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"
#include "reflection/argument/argument.h"

namespace reflection
{

bool Property::Vaild() const
{
	return wrapper->Vaild();
}

std::string const &Property::Name() const
{
	return wrapper->Name();
}

Class Property::DeclaringClass() const
{
	return wrapper->DeclaringClass();
}

bool Property::SetValue(Variant &obj, Argument arg)
{
	return wrapper->SetValue(obj, arg);
}

Variant Property::Value(Variant &obj)
{
	return wrapper->Value(obj);
}

}



