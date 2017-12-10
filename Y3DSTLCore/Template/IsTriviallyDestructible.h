#pragma once

namespace IsTriviallyDestructible_Private
{
	// We have this specialization for enum to avoid the need to have a full definition of
	// the type.
	template <typename T, bool bIsTriviallyDestructible = __is_enum(T)>
	struct Impl
	{
		enum { Value = true };
	};

	template <typename T>
	struct Impl<T, false>
	{
		enum { Value = __has_trivial_destructor(T) };
	};
};

/**
* Traits class which tests if a type has a trivial destructor.
*/
template <typename T>
struct IsTriviallyDestructible
{
	enum { Value = IsTriviallyDestructible_Private::Impl<T>::Value };
};