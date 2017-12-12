#pragma once

#include "Template/IsPODType.h"
#include "Template/AndOrNot.h"
#include "Template/AddReference.h"

/**
* Traits class which tests if a type has a trivial class.
*/
template<class Type>
struct IsTrivial
{
	enum { Value = __is_trivial(Type) };
};

/**
* Traits class which tests if a type has a trivial copyable class.
*/
template<class Type>
struct IsTriviallyCopyable
{
	enum { Value = __is_trivially_copyable(Type) };
};


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

/**
* Traits class which tests if a type has a trivial destructor.
*
* A trivially destructible class is a class (defined with class, struct or union) that :
*
*	uses the implicitly defined destructor.
*	the destructor is not virtual.
*	its base class and non - static data members(if any) are themselves also trivially destructible types.
*/
namespace IsTriviallyDestructible_Private
{
	// We have this specialization for enum to avoid the need to have a full definition of
	// the type.
	template <typename T, bool bIsTriviallyDestructible = __is_enum(T)>
	struct Impl
	{
		enum { Value = true };
	};

	template <typename T>
	struct Impl<T, false>
	{
		enum { Value = __has_trivial_destructor(T) };
	};
};

/**
* Traits class which tests if a type has a trivial destructor.
*/
template <typename T>
struct IsTriviallyDestructible
{
	enum { Value = IsTriviallyDestructible_Private::Impl<T>::Value };
};

