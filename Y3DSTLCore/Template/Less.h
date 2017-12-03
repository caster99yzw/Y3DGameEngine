#pragma once

#include "../CoreTypes.h"

/**
* Binary predicate class for sorting elements in order.  Assumes < operator is defined for the template type.
* Forward declaration exists in ContainersFwd.h
*
* 
*/
template <typename T>
struct Less
{
	FORCEINLINE bool operator()(T const& A, T const& B) const
	{
		return A < B;
	}
};

template <>
struct Less<void>
{
	template <typename T>
	FORCEINLINE bool operator()(T const& A, T const& B) const
	{
		return A < B;
	}
};