#pragma once

#include "CoreTypes.h"

/**
* Binary predicate class for sorting elements in reverse order.  Assumes < operator is defined for the template type.
*
* 
*/
template <typename T = void>
struct Greater
{
	FORCEINLINE bool operator()(T const& A, T const& B) const
	{
		return A > B;
	}
};

template <>
struct Greater<void>
{
	template <typename T>
	FORCEINLINE bool operator()(T const& A, T const& B) const
	{
		return A > B;
	}
};