#pragma once

namespace reflection
{

template <typename T>
Argument::Argument(T &value)
	: value(reinterpret_cast<void const *>(std::addressof(value)))
{
}

template <typename T>
Argument::Argument(T const &value)
	: value(reinterpret_cast<void const *>(std::addressof(value)))
{
}

template <typename T>
std::enable_if_t<std::is_pointer_v<T>, bool> Argument::IsType() const
{
	return false;
}

template <typename T>
std::enable_if_t<!std::is_pointer_v<T>, bool> Argument::IsType() const
{
	return false;
}

template <typename T>
std::enable_if_t<!std::is_rvalue_reference_v<T>, T> &Argument::Value() const
{
	using RawT = std::remove_reference_t<T>;
	return (*reinterpret_cast<RawT *>(const_cast<void *>(value)));
}

template <typename T>
std::enable_if_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>> &&Argument::Value() const
{
	using RawT = std::remove_reference_t<T>;
	return std::move(*reinterpret_cast<RawT *>(const_cast<void *>(value)));
}

}

