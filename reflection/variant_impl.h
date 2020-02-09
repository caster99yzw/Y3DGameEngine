#pragma once

namespace reflection
{

template <typename T, typename Tp>
reflection::Variant::Variant(T &&inValue)
{
	value = new Tp(inValue);
}

template <typename T>
T& reflection::Variant::Value()
{
	return  *reinterpret_cast<T *>(value);
}

template <typename T>
T const& reflection::Variant::Value() const
{
	return  *reinterpret_cast<T const *>(value);
}

template <typename T>
T *reflection::Variant::ValuePtr() const
{
	return  reinterpret_cast<T *>(const_cast<void *>(value));
}

}

