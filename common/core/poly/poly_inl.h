#pragma once
#include "common/core/overload.h"
#include "common/core/type_traits.h"
#include "common/core/aligned_type.h"

namespace common {

template <auto...>
struct PolyMembers;

namespace impl {
	
struct PolyBase {};

struct Data
{
	Data() = default;
	Data(const Data&) {}
	Data& operator=(const Data&) { return *this; }
	union {
		void* Object = nullptr;
		TypeAlignedBytes<double[2]> Buffer;
	};
};

template <typename I>
struct PolyRoot : private PolyBase, private Data
{
	friend struct PolyAccess;
	using PolyInterface = I;
	
private:
};

} // namespace impl 
} // namespace common
