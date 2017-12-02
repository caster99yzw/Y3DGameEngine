#include "EnableIf.h"
#include "../CoreTypes.h"


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
template <typename T, typename = typename EnableIf<EnableIf<IsContiguousContainer<T>::Value>::Type>::Type>
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
