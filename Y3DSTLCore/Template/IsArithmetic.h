#pragma once

#include "CoreTypes.h"

/**
* Traits class which tests if a type is arithmetic.
*/
template <typename T>
struct IsArithmetic
{
	enum { Value = false };
};

template <> struct IsArithmetic<float> { enum { Value = true }; };
template <> struct IsArithmetic<double> { enum { Value = true }; };
template <> struct IsArithmetic<long double> { enum { Value = true }; };
template <> struct IsArithmetic<UINT8> { enum { Value = true }; };
template <> struct IsArithmetic<UINT16> { enum { Value = true }; };
template <> struct IsArithmetic<UINT32> { enum { Value = true }; };
template <> struct IsArithmetic<UINT64> { enum { Value = true }; };
template <> struct IsArithmetic<INT8> { enum { Value = true }; };
template <> struct IsArithmetic<INT16> { enum { Value = true }; };
template <> struct IsArithmetic<INT32> { enum { Value = true }; };
template <> struct IsArithmetic<INT64> { enum { Value = true }; };
template <> struct IsArithmetic<bool> { enum { Value = true }; };
template <> struct IsArithmetic<ANSICHAR> { enum { Value = true }; };
template <> struct IsArithmetic<WIDECHAR> { enum { Value = true }; };

template <typename T> struct IsArithmetic<const          T> { enum { Value = IsArithmetic<T>::Value }; };
template <typename T> struct IsArithmetic<      volatile T> { enum { Value = IsArithmetic<T>::Value }; };
template <typename T> struct IsArithmetic<const volatile T> { enum { Value = IsArithmetic<T>::Value }; };