#pragma once

#include "Y3DPI.h"

#include <cmath>
#include <limits>

// Provide the interface to cover the type of data
// Function of c-math can not be non-constexpr 

namespace Y3D
{
	// Basic Function
	template <class T>
	static T Abs(T const& rhs) { return static_cast<T>(std::fabs(rhs)); }

	template <class T>
	static T Sqrt(T const& lhs) { return static_cast<T>(std::sqrt(lhs)); }

	template <class T>
	static T RecipSqrt(T const& lhs) { return T(1) / static_cast<T>(std::sqrt(lhs)); }

	template <class T>
	static T Mod(T const& lhs, T const& rhs) { return static_cast<T>(std::fmod(lhs, rhs)); }

	// Basic self-definite function
	template <class T>
	constexpr static T Sign(T const& rhs) { return rhs >= T(0) ? T(1) : T(-1); }

	template <class T>
	constexpr static T Max(T const& lhs, T const& rhs) { return lhs < rhs ? rhs : lhs; }

	template <class T>
	constexpr static T Min(T const& lhs, T const& rhs) { return lhs > rhs ? rhs : lhs; }

	template <class T>
	constexpr static T Clamp(T const& rhs, T const& low, T const& hi)
	{
		return (rhs > hi) ? hi : ((rhs < low) ? low : rhs);
	}

	template <class T>
	constexpr static T Quadric(T const& rhs) { return rhs * rhs; }

	template <class T>
	constexpr static T Cubic(T const& rhs) { return rhs * rhs * rhs; }

	// Equi Function
	template <class T>
	constexpr static BOOL Equi(T const& lhs, T const& rhs) { return lhs == rhs; }

	template <>
	static BOOL Equi<FLOAT32>(FLOAT32 const& lhs, FLOAT32 const& rhs)
	{
		return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT32>::epsilon();
	}

	template <>
	static BOOL Equi<FLOAT64>(FLOAT64 const& lhs, FLOAT64 const& rhs)
	{
		return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT64>::epsilon();
	}

	template <class T>
	static BOOL Equal(Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return std::abs(lhs.radValue - rhs.radValue) <= std::numeric_limits<T>::epsilon();
	}

	template <class T>
	static BOOL Equal(Degree<T> const& lhs, Degree<T> const& rhs)
	{ 
		return std::abs(lhs.degVaule - rhs.degVaule) <= std::numeric_limits<T>::epsilon();
	}

	// Trigonometric Function
	template <class T>
	static T Sin(Radian<T> const& rhs) { return static_cast<T>(std::sin(rhs.radValue)); }

	template <class T>
	static T Cos(Radian<T> const& rhs) { return static_cast<T>(std::cos(rhs.radValue)); }

	template <class T>
	static T Tan(Radian<T> const& rhs) { return static_cast<T>(std::tan(rhs.radValue)); }

	template <class T>
	static Radian<T> ASin(T const& rhs) 
	{ 
		return Radian<T>(
			static_cast<T>(std::asin(rhs))
			); 
	}

	template <class T>
	static Radian<T> ACos(T const& rhs) 
	{ 
		return Radian<T>(
			static_cast<T>(std::acos(rhs))
			); 
	}

	template <class T>
	static Radian<T> ATan(T const& rhs)
	{
		return Radian<T>(
			static_cast<T>(std::atan(rhs))
			); 
	}

	template <class T>
	static Radian<T> ATan2(T const& rhs0, T const& rhs1)
	{ 
		return Radian<T>(
			static_cast<T>(std::atan2(rhs0, rhs1))
			); 
	}

	// Logarithmic and Exponential Function
	template <class T>
	static T LogE(T const& rhs) { return static_cast<T>(std::log(rhs)); }

	template <class T>
	static T Log2(T const& rhs) { return static_cast<T>(std::log2(rhs)); }

	template <class T>
	static T Log10(T const& rhs) { return static_cast<T>(std::log10(rhs)); }

	template <class T>
	static T Pow(T const& rhs, T const& ex) { return static_cast<T>(std::pow(rhs, ex)); }

	template <class T>
	static T Exp(T const& ex) { return static_cast<T>(std::exp(ex)); }
}