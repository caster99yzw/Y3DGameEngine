#pragma once

#include "CoreTypes.h"
#include "Template/AndOrNot.h"
#include "Template/IsPODType.h"

/**
* Traits class which tests if a type has a trivial copy assignment operator.
*/
template <typename Type>
struct IsTriviallyCopyAssignable
{
	enum { Value = OrVaule<__has_trivial_assign(Type), IsPODType<Type>>::Value};
};