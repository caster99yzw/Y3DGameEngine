#pragma once

#include "CoreTypes.h"
#include "Template/AndOrNot.h"
#include "Template/IsPODType.h"
#include "Template/AddReference.h"

/**
* Traits class which tests if a type has a trivial constructor.
*	
* Both T and all the types in Arguments must be complete types, void, or arrays of unknown bound.
*/
template<typename Type, typename... Args>
struct IsTriviallyConstructible
{
	enum { Value = OrVaule<__is_trivially_constructible(Type, Args...), IsPODType<Type>>::Value };
};

/**
* Traits class which tests if a type has a trivial copy constructor.
*
* A copy constructor for a class T is trivial if :
* 
*	it is implicitly declared.
*	the class T has no virtual functions or virtual bases.
*	all the direct bases of class T have trivial copy constructors.
*	the classes of all the non - static data members of class type have trivial copy constructors.
*	the classes of all the non - static data members of type array of class have trivial copy constructors.
*/
template<typename Type>
struct IsTriviallyCopyConstructible 
{	
	enum {	Value = IsTriviallyConstructible<Type, AddLValueReference<Type const>>::Value };
};

/**
* Traits class which tests if a type has a trivial default constructor.
*
* A default constructor for a class Ty is trivial if :
*
*	it is an implicitly declared default constructor
*	the class Ty has no virtual functions
*	the class Ty has no virtual bases
*	all the direct bases of the class Ty have trivial constructors
*	the classes of all the non - static data members of class type have trivial constructors
*	the classes of all the non - static data members of type array of class have trivial constructors
*/
template<typename _Ty>
struct IsTriviallyDefaultConstructible
{
	enum { Value = IsTriviallyConstructible<Type>::Value };
};

/**
* Traits class which tests if a type has a trivial move constructor.
*
* A move constructor for a class Ty is trivial if :
*
*	it is implicitly declared
*	its parameter types are equivalent to those of an implicit declaration
*	the class Ty has no virtual functions
*	the class Ty has no virtual bases
*	the class has no volatile non - static data members
*	all the direct bases of the class Ty have trivial move constructors
*	the classes of all the non - static data members of class type have trivial move constructors
*	the classes of all the non - static data members of type array of class have trivial move constructors
*/
template<typename _Ty>
struct IsTriviallyMoveConstructible
{
	enum { Value = IsTriviallyConstructible<Type, Type>::Value };
};
