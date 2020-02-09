#include "destructor.h"
#include "reflection/class/class.h"
#include "reflection/variant.h"

namespace reflection
{

bool Destructor::Valid() const
{
	return wrapper->Valid();
}

Class Destructor::DeclaringClass() const
{
	return wrapper->DeclaringClass();
}

bool Destructor::Invoke(Variant obj) const
{
	return wrapper->Invoke(obj);
}

}

