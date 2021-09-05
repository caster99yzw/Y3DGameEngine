#pragma once
#include "common/core/overload.h"
#include "common/core/type_traits.h"

namespace common {
	
template <auto... Ps>
struct PolyMembers {};

template <class I>
struct Poly final : detail::PolyValOrRef<I> {
	friend detail::PolyAccess;
	Poly() = default;
	using detail::PolyValOrRef<I>::PolyValOrRef;
	using detail::PolyValOrRef<I>::operator=;
};
	
} // namespace common
