#include "method.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"
#include "reflection/argument/argument.h"

namespace reflection
{


bool Method::Valid() const
{
	return wrapper->Vaild();
}

std::string const &Method::Name() const
{
	return wrapper->Name();
}

common::array_view<reflection::Parameter> Method::Parameters() const
{
	return wrapper->Parameters();
}

Variant Method::Invoke(Variant &obj) const
{
	return wrapper->Invoke(obj);
}

Variant Method::Invoke(Variant &obj, Argument arg0) const
{
	return wrapper->Invoke(obj, arg0);
}

Variant Method::Invoke(Variant &obj, Argument arg0, Argument arg1) const
{
	return wrapper->Invoke(obj, arg0, arg1);
}

Variant Method::Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2) const
{
	return wrapper->Invoke(obj, arg0, arg1, arg2);
}

Variant Method::Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3) const
{
	return wrapper->Invoke(obj, arg0, arg1, arg2, arg3);
}

Variant Method::Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4) const
{
	return wrapper->Invoke(obj, arg0, arg1, arg2, arg3, arg4);
}

Variant Method::Invoke(Variant &obj, Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4, Argument arg5) const
{
	return wrapper->Invoke(obj, arg0, arg1, arg2, arg3, arg4, arg5);
}

Variant Method::Invoke(Variant &obj, std::vector<Argument> args) const
{
	return wrapper->Invoke(obj, args);
}

}

