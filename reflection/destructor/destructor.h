#pragma once
#include "destructor_wrapper.h"

namespace reflection
{

class Destructor
{
public:
	Destructor() = default;
	Destructor(DestructorWrapperBase const *inWrapper)
		: wrapper(inWrapper) {}

	bool Valid() const;
	Class DeclaringClass() const;
	bool Invoke(Variant obj) const;

private:
	DestructorWrapperBase const *wrapper = nullptr;
};

}



