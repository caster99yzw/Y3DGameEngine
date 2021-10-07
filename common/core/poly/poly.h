#pragma once
#include "common/core/overload.h"
#include "common/core/type_traits.h"
#include "common/core/poly/poly_detail.h"

namespace common {
	
template <auto... Mems>
struct PolyMembers {};

namespace impl {
template <class I>
struct PolyVal : PolyImpl<I>
{
public:
	PolyVal() = default;
	PolyVal(PolyVal&& other);
	~PolyVal();

	template <typename T, typename = std::enable_if_t<InterfaceMatching<T, I>::value>>
	PolyVal(T&& t);

private:
	using Copyable = std::is_copy_constructible<PolyImpl<I>>;
	PolyRoot<I>& GetRoot() { return *this; }
	const PolyRoot<I>& GetRoot() const { return *this; }
	Data* GetData() { return PolyAccess::data(*this); }
	const Data* GetData() const { return PolyAccess::data(*this); }
};

template <class I>
PolyVal<I>::PolyVal(PolyVal&& other)
{
	other.m_vptr->m_ops(Op::Move, &other, static_cast<Data*>(this));
	m_vptr = std::exchange(other.m_vptr, GetVtable<I>());
}

template <class I>
PolyVal<I>::~PolyVal()
{
	m_vptr->m_ops(Op::Nuke, this, nullptr);
}

template <class I>
template <typename T, typename>
PolyVal<I>::PolyVal(T&& t)
{
	using U = std::decay_t<T>;
	static_assert(std::is_copy_constructible<U>::value || !Copyable::value,
		"This Poly<> requires copyability, and the source object is not "
		"copyable");
	assert(typeid(t) == typeid(std::decay_t<T>) ||
		!"Dynamic and static exception types don't match. Object would "
		"be sliced when storing in Poly.");

	if (IsInSitu<U>()) {
		auto const buff = static_cast<void*>(&(GetData()->Buffer));
		::new (buff) U(static_cast<T&&>(t));
	}
	else {
		GetData()->Object = new U(static_cast<T&&>(t));
	}
	m_vptr = GetVtable<I, U>();
}
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
