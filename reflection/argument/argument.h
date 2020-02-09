#pragma once
#include "reflection/core.h"

namespace reflection
{

class Argument
{
public:
	Argument() = default;

	template <typename T>
	Argument(T &value);
	template <typename T>
	Argument(T const &value);

	template <typename T>
	std::enable_if_t<std::is_pointer_v<T>, bool> IsType() const;
	template <typename T>
	std::enable_if_t<!std::is_pointer_v<T>, bool> IsType() const;

	template <typename T>
	std::enable_if_t<!std::is_rvalue_reference_v<T>, T> &Value() const;
	template <typename T>
	std::enable_if_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>> &&Value() const;

private:
	void const *value = nullptr;
};

}

#include "argument_impl.h"
