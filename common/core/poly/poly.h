#pragma once
#include "common/core/overload.h"
#include "common/core/type_traits.h"
#include "common/core/poly/poly_inl.h"

namespace common {
	
template <auto... Mems>
struct PolyMembers {};

namespace impl {
template <class I>
struct PolyVal : PolyImpl<I> {
};
} // namespace impl

template <std::size_t N, typename This, typename... As>
auto poly_call(This&& t, As&&... as)
	-> decltype(impl::PolyAccess::call<N>(
		static_cast<This&&>(t), static_cast<As&&>(as)...))
{
	return impl::PolyAccess::call<N>(
		static_cast<This&&>(t), static_cast<As&&>(as)...); 
}

template <class I>
struct Poly final : impl::PolyVal<I> {
	Poly() = default;
	using impl::PolyVal<I>::PolyVal;
	using impl::PolyVal<I>::operator=;
};
	
} // namespace common
