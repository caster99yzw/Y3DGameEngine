#pragma once

#include "RemoveReference.h"
#include "RemoveCV.h"

namespace Decay_Private
{
	template <typename T>
	struct DecayNonReference
	{
		typedef typename RemoveCV<T>::Type Type;
	};
	
	template <typename T>
	struct DecayNonReference<T[]>
	{
		typedef typename T* Type;
	};

	template <typename T, int N>
	struct DecayNonReference<T[N]>
	{
		typedef typename T* Type;
	};

	template <typename RetType, typename... Param>
	struct DecayNonReference<RetType(Param...)>
	{
		typedef RetType (*Type)(Param...);
	};
}

/**
* Returns the decayed type of T, meaning it removes all references, qualifiers and
* applies array-to-pointer and function-to-pointer conversions.
*
*/
template <typename T>
struct Decay
{
	typedef typename Decay_Private::DecayNonReference<typename RemoveReference<T>::Type>::Type Type;
};
