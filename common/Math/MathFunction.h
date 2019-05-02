#pragma once
#include "../Type.h"

namespace y3dmath
{
	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	FORCEINLINE constexpr T Max(T lhs, T rhs) 
	{
		return lhs < rhs ? rhs : lhs; 
	}

	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	FORCEINLINE constexpr T Min(T lhs, T rhs) 
	{
		return lhs > rhs ? rhs : lhs; 
	}

	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	FORCEINLINE constexpr T Clamp(T value, T min, T max)
	{
		T v = Min(value, max);
		return Max(v, min);
	}

	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	FORCEINLINE constexpr T Ratio(T value, T min, T max)
	{
		return value - min / max - min;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE constexpr T Pow(T value, T exponent)
	{
		return pow(value, exponent);
	}
}

