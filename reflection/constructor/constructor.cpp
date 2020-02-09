#include "constructor.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"
#include "reflection/argument/argument.h"

namespace reflection
{

bool Constructor::Vaild() const
{
	return wrapper->Vaild();
}

Class Constructor::DeclaringClass() const
{
	return wrapper->DeclaringClass();
}

common::array_view<Parameter> Constructor::Parameters() const
{
	return wrapper->Parameters();
}

Variant Constructor::Invoke() const
{
	return wrapper->Invoke();
}

Variant Constructor::Invoke(Argument arg0) const
{
	return wrapper->Invoke(arg0);
}

Variant Constructor::Invoke(Argument arg0, Argument arg1) const
{
	return wrapper->Invoke(arg0, arg1);
}

Variant Constructor::Invoke(Argument arg0, Argument arg1, Argument arg2) const
{
	return wrapper->Invoke(arg0, arg1, arg2);
}

Variant Constructor::Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3) const
{
	return wrapper->Invoke(arg0, arg1, arg2, arg3);
}

Variant Constructor::Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4) const
{
	return wrapper->Invoke(arg0, arg1, arg2, arg3, arg4);
}

Variant Constructor::Invoke(Argument arg0, Argument arg1, Argument arg2, Argument arg3, Argument arg4, Argument arg5) const
{
	return wrapper->Invoke(arg0, arg1, arg2, arg3, arg4, arg5);
}

Variant Constructor::Invoke(std::vector<Argument> args) const
{
	return wrapper->Invoke(args);
}

}
