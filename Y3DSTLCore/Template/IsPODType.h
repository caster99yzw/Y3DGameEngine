#pragma once

#include "CoreTypes.h"
#include "Template/IsPointer.h"
#include "Template/AndOrNot.h"
#include "Template/IsArithmetic.h"

/**
* Traits class which tests if a type is POD.
*
* Arithmetic types, enumeration types, pointer types, and pointer to member types are POD.
*
* A const or volatile qualified version of a POD type is itself a POD type.
*
* An array of POD is itself POD.
*
* A struct or union, all of whose non-static data members are POD, is itself POD if it has:
*
*	No user-declared constructors.
*	No private or protected non-static data members.
*	No base classes.
*	No virtual functions.
*	No non-static data members of reference type.
*	No user-defined copy assignment operator.
*	No user-defined destructor.
*
* Therefore, you can recursively build POD struct and array that contain POD struct and array.
*/

template <typename Type>
struct IsPODType
{
	enum { Value = OrVaule<__is_pod(Type) | __is_enum(Type), IsArithmetic<Type>, IsPointer<Type>>::Value };
};