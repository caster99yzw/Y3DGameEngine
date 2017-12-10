#pragma once

#include "CoreTypes.h"
#include "Template/AndOrNot.h"
#include "Template/IsPODType.h"

/**
* Traits class which tests if a type has a trivial copy constructor.
*/
template <typename T>
struct IsTriviallyCopyConstructible
{
	enum { Value = OrVaule<__has_trivial_copy(T), IsPODType<T>>::Value };
};
