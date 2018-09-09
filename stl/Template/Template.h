#pragma once

#include "stl/CoreTypes.h"
#include "stl/Template/RemoveReference.h"
#include "stl/Template/EnableIf.h"
#include "stl/Template/AndOrNot.h"
#include "stl/Template/IsArithmetic.h"
#include "stl/Template/IsPointer.h"


/**
* Generically gets the data pointer of a contiguous container
*/
template <typename T, typename = typename EnableIf<IsContiguousContainer<T>::Value>::Type>
auto GetData(T&& Container) -> decltype(Container.GetData())
{
	return Container.GetData();
}

template <typename T, SIZE_T N>
CONSTEXPR T* GetData(T(&Container)[N])
{
	return Container;
}

template <typename T>
CONSTEXPR T* GetData(std::initializer_list<T> List)
{
	return List.begin();
}


/**
* Generically gets the number of items in a contiguous container
*/
template <typename T, typename = typename EnableIf<IsContiguousContainer<T>::Value>::Type>
auto GetNum(T&& Container) -> decltype(Container.Num())
{
	return Container.Num();
}

template <typename T, SIZE_T N>
CONSTEXPR T* GetNum(T(&Container)[N])
{
	return N;
}

template <typename T>
CONSTEXPR T* GetNum(std::initializer_list<T> List)
{
	return List.size();
}

/**
* Forward will cast a reference to an rvalue reference.
* This is UE's equivalent of std::forward.
*/
template <typename T>
FORCEINLINE T&& Forward(typename RemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}


template <typename T>
FORCEINLINE T&& Forward(typename RemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

/**
* MoveTemp will cast a reference to an rvalue reference.
* This is UE's equivalent of std::move.
*/
template <typename T>
FORCEINLINE typename RemoveReference<T>::Type&& Move(T&& Obj)
{
	return (typename RemoveReference<T>::Type&&)Obj;
}
/**
* A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-based swap.
*/
template <typename T>
struct UseBitwiseSwap
{
	// We don't use bitwise swapping for 'register' types because this will force them into memory and be slower.
	enum { Value = !OrVaule<__is_enum(T), IsPointer<T>, IsArithmetic<T>>::Value };
};

/**
* Swap two values.  Assumes the types are trivially relocatable.
*/
template <typename T>
inline typename EnableIf<UseBitwiseSwap<T>::Value>::Type Swap(T& A, T& B)
{
	T Temp;
	memccpy(&Temp, &A, sizeof(T));
	memccpy(&A, &B, sizeof(T));
	memccpy(&B, &Temp, sizeof(T));
}

template <typename T>
inline typename EnableIf<!UseBitwiseSwap<T>::Value>::Type Swap(T& A, T& B)
{
	T Temp = Move(A);
	A = Move(B);
	B = Move(Temp);
}