#pragma once

#include "../CoreTypes.h"

/**
* Traits class which tests if a type is a contiguous container.
* Requires:
*    [ &Container[0], &Container[0] + Num ) is a valid range
*/
template <typename T>
struct IsContiguousContainer
{
	enum { Value = false };
};

template <typename T> struct IsContiguousContainer<             T& > : IsContiguousContainer<T> {};
template <typename T> struct IsContiguousContainer<             T&&> : IsContiguousContainer<T> {};
template <typename T> struct IsContiguousContainer<const          T> : IsContiguousContainer<T> {};
template <typename T> struct IsContiguousContainer<      volatile T> : IsContiguousContainer<T> {};
template <typename T> struct IsContiguousContainer<const volatile T> : IsContiguousContainer<T> {};

/**
* Specialization for C arrays (always contiguous)
*/
template <typename T, size_t N> struct IsContiguousContainer<               T[N]> { enum { Value = true }; };
template <typename T, size_t N> struct IsContiguousContainer<const          T[N]> { enum { Value = true }; };
template <typename T, size_t N> struct IsContiguousContainer<      volatile T[N]> { enum { Value = true }; };
template <typename T, size_t N> struct IsContiguousContainer<const volatile T[N]> { enum { Value = true }; };

/**
* Specialization for initializer lists (also always contiguous)
*/
template <typename T>
struct IsContiguousContainer<std::initializer_list<T>>
{
	enum { Value = true };
};
