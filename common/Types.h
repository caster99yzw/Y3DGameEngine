#pragma once
#include <type_traits>
#include <stdint.h>
#include <assert.h>

#define FORCEINLINE __forceinline

using uint8			= uint8_t;
using int8			= int8_t;
using uint16		= uint16_t;
using int16			= int16_t;
using uint32		= uint32_t;
using int32			= int32_t;
using uint64		= uint64_t;
using int64			= int64_t;
using ascichar		= char;
using widechar		= wchar_t;

using float32		= float;
using float64		= double;

template <typename T> struct is_pod_array  { constexpr static bool Value = false; };
template <typename T> struct is_pod_array<T[]>  { constexpr static bool Value = true; };
template <typename T, std::size_t N> struct is_pod_array<T[N]>  { constexpr static bool Value = true; };

template <typename T> struct is_unbounded_array  { constexpr static bool Value = false; };
template <typename T> struct is_unbounded_array<T[]>  { constexpr static bool Value = true; };

template <typename T> struct is_bounded_array  { constexpr static bool Value = false; };
template <typename T, std::size_t N> struct is_bounded_array<T[N]>  { constexpr static bool Value = true; };

#define ONLY_MOVABLE_FOR_CLASS(cls) \
	cls(cls&&) = default; \
	cls& operator=(cls&&) = default; \
	cls(const cls&) = delete; \
	cls& operator=(const cls&) = delete

#define ONLY_COPYABLE_FOR_CLASS(cls) \
	cls(cls&&) = delete; \
	cls& operator=(cls&&) = delete; \
	cls(const cls&) = default; \
	cls& operator=(const cls&) = default

#define MOVABLE_AND_COPYABLE_FOR_CLASS(cls) \
	cls(cls&&) = default; \
	cls& operator=(cls&&) = default; \
	cls(const cls&) = default; \
	cls& operator=(const cls&) = default

#define NOT_MOVABLE_AND_COPYABLE_FOR_CLASS(cls) \
	cls(cls&&) = delete; \
	cls& operator=(cls&&) = delete; \
	cls(const cls&) = delete; \
	cls& operator=(const cls&) = delete
	
template <typename T, typename U>
using conv = std::is_convertible<U, T>;


