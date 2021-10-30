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
	PolyVal() = default;
	PolyVal(PolyVal&& other) noexcept;
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
struct PolyRef : private PolyImpl<I>
{
	PolyRef() = default;
	PolyRef(const PolyRef& other);
	~PolyRef() = default;

	template <typename T, typename = std::enable_if_t<InterfaceMatching<T, I>::value>>
	PolyRef(T&& t);

	AddCvrefOf<PolyImpl<I>, I>& operator*() const { return GetPtr(); }
	auto operator->() const { return &GetPtr(); }

private:
	AddCvrefOf<PolyRoot<I>, I>& GetRoot() const;
	AddCvrefOf<PolyImpl<I>, I>& GetPtr() const;
	Data* GetData() { return PolyAccess::data(*this); }
	const Data* GetData() const { return PolyAccess::data(*this); }
};

template <typename I>
using PolyValOrRef = If<std::is_reference<I>::value, PolyRef<I>, PolyVal<I>>;

template <class I>
PolyVal<I>::PolyVal(PolyVal&& other) noexcept
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
	assert(typeid(t) == typeid(std::decay_t<T>));

	if (IsInSitu<U>()) {
		auto const buff = static_cast<void*>(&(GetData()->Buffer));
		::new (buff) U(static_cast<T&&>(t));
	}
	else {
		GetData()->Object = new U(static_cast<T&&>(t));
	}
	m_vptr = GetVtable<I, U>();
}

template <class I>
PolyRef<I>::PolyRef(const PolyRef& other)
{
	GetData()->Object = other.GetData()->Object;
	this->m_vptr = other.m_vptr;
}

template <class I>
template <typename T, typename>
PolyRef<I>::PolyRef(T&& t)
{
	GetData()->Object = const_cast<void*>(
		static_cast<void const*>(std::addressof(t)));

	this->m_vptr = GetVtable<std::decay_t<I>,
		AddCvrefOf<std::decay_t<T>, I>>();
}

template <class I>
AddCvrefOf<PolyRoot<I>, I>& PolyRef<I>::GetRoot() const
{
	return const_cast<AddCvrefOf<PolyRoot<I>, I>&>(
		static_cast<const PolyRoot<I>&>(*this));
}

template <class I>
AddCvrefOf<PolyImpl<I>, I>& PolyRef<I>::GetPtr() const
{
	return const_cast<AddCvrefOf<PolyImpl<I>, I>&>(
		static_cast<PolyImpl<I> const&>(*this));
}
} // namespace impl

template <std::size_t N, typename This, typename... As>
auto PolyCall(This&& t, As&&... as)
-> decltype(impl::PolyAccess::call<N>(
	static_cast<This&&>(t), static_cast<As&&>(as)...))
{
	return impl::PolyAccess::call<N>(
		static_cast<This&&>(t), static_cast<As&&>(as)...);
}

template <typename T, typename I>
AddCvrefOf<T, I>& PolyCast(impl::PolyVal<I>& that)
{
	return impl::PolyAccess::cast<T>(
		static_cast<impl::PolyRoot<I>&>(that));
}

template <typename T, typename I>
const AddCvrefOf<T, I>& PolyCast(const impl::PolyVal<I>& that)
{
	return impl::PolyAccess::cast<T>(
		static_cast<const impl::PolyRoot<I>&>(that));
}

template <typename T, typename I>
AddCvrefOf<T, I>& PolyCast(impl::PolyRef<I>& that)
{
	return impl::PolyAccess::cast<T>(*that);
}

template <typename T, typename I>
const AddCvrefOf<T, I>& PolyCast(const impl::PolyRef<I>& that)
{
	return impl::PolyAccess::cast<T>(*that);
}

template <class... I>
struct PolyExtends : virtual I...
{
	using Subsumptions = TypeList<I...>;

	template <typename Base>
	struct Interface : Base
	{
		Interface() = default;
		using Base::Base;
	};

	template <typename...>
	using Members = PolyMembers<>;
};

template <class I>
struct Poly final : impl::PolyValOrRef<I>
{
	Poly() = default;
	using impl::PolyValOrRef<I>::PolyValOrRef;
	using impl::PolyValOrRef<I>::operator=;
};
	
} // namespace common
