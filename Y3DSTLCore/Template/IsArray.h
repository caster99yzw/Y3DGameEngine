#pragma once

#include "CoreTypes.h"

/**
* Traits class which tests if a type is a C++ array.
*/
template <typename T>           struct IsArray			{ enum { Value = false }; };
template <typename T>           struct IsArray<T[]>		{ enum { Value = true }; };
template <typename T, UINT32 N> struct IsArray<T[N]>	{ enum { Value = true }; };