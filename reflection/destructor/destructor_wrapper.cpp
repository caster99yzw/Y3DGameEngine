#include "destructor_wrapper.h"
#include "reflection/class/class.h"

namespace reflection
{

bool DestructorWrapperBase::Valid() const
{
	return false;
}

Class DestructorWrapperBase::DeclaringClass() const
{
	return Class();
}

bool DestructorWrapperBase::Invoke(Variant &obj) const
{
	return false;
}

}

