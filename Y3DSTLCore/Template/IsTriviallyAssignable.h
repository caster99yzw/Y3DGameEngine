#pragma once

#include "CoreTypes.h"
#include "Template/AndOrNot.h"
#include "Template/IsPODType.h"
#include "Template/AddReference.h"

/**
* Traits class which tests if a type has a trivial assignment.
*
* Both From and To must be complete types, void, or arrays of unknown bound.
*/
template <typename To, typename From>
struct IsTriviallyAssignable
{
	enum { Value = OrVaule<__is_trivially_assignable(To, From), IsPODType<To>>::Value };
};

/**
* Traits class which tests if a type has a trivial copy assignment.
*
* An assignment constructor for a class T is trivial if :
*	
*	it is implicitly provided,
*	the class T has no virtual functions
*	the class T has no virtual bases
*	the classes of all the non-static data members of class type have trivial assignment operators
*	the classes of all the non-static data members of type array of class have trivial assignment operators.
*/
template<typename Type>
struct IsTriviallyCopyAssignable
{
	enum { Value = IsTriviallyAssignable<AddLValueReference<Type>, AddLValueReference<Type const>>::Value };
};

/**
* Traits class which tests if a type has a trivial move assignment.
*
* A move assignment operator for a class Ty is trivial if:
*
*	it is implicitly provided
*	the class Ty has no virtual functions
*	the class Ty has no virtual bases
*	the classes of all the non-static data members of class type have trivial move assignment operators
*	the classes of all the non-static data members of type array of class have trivial move assignment operators
*/
template<typename Type>
struct IsTriviallyMoveAssignable
{
	enum { Value = IsTriviallyAssignable<AddLValueReference<Type>, Type>::Value };
};
