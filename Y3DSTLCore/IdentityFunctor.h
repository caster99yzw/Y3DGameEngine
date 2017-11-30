#pragma once

#define FORCEINLINE __forceinline
#define INDEX_NONE -1
typedef unsigned long		SIZE_T;

struct FIdentityFunctor
{
	template <typename T>
	FORCEINLINE T&& operator()(T&& Val) const
	{
		return (T&&)Val;
	}
};
