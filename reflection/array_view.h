#pragma once
#include "core.h"

namespace common
{

template <typename T>
class array_view
{
public:
	array_view() = default;
	array_view(T const *inData, std::size_t inCount)
		: data(inData), count(inCount) {}

	std::size_t size() { return count; }
		
private:
	T const *data = nullptr;
	std::size_t count = 0;
};

}

