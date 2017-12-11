#pragma once

#include "CoreTypes.h"

/**
* Traits class which tests if a type is a pointer.
*/
template <typename T>
struct IsPointer
{
	enum { Value = false };
};

template <typename T> struct IsPointer<T*>				 { enum { Value = true }; };

template <typename T> struct IsPointer<const          T> { enum { Value = IsPointer<T>::Value }; };
template <typename T> struct IsPointer<      volatile T> { enum { Value = IsPointer<T>::Value }; };
template <typename T> struct IsPointer<const volatile T> { enum { Value = IsPointer<T>::Value }; };
