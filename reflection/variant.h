#pragma once
#include "reflection/core.h"

namespace reflection
{

class Variant
{
public:
	Variant() = default;

	template <typename T, typename Tp = std::remove_reference_t<T>>
	Variant(T &&inValue);

	template <typename T>
	T& Value();

	template <typename T>
	T const& Value() const;

	template <typename T>
	T *ValuePtr() const;

private:
	void *value;
};

}

#include "variant_impl.h"

