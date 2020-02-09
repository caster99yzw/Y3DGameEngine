#pragma once
#include "reflection/destructor/destructor_wrapper.h"

namespace reflection
{

template <typename ClassType>
struct DestructorWrapper : public DestructorWrapperBase
{
public:
	bool Valid() const override { return true; }
	Class DeclaringClass() const { return Class::Get<ClassType>(); }

	bool Invoke(Variant &obj) const override
	{
		obj = Variant();
		return true;
	}
};

}

