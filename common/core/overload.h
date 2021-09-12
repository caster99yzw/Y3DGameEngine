#pragma once
#include "common/Types.h"

namespace common {

namespace impl {
template <typename Fun>
struct Overload
{
	template <typename T>
	constexpr Fun T::* operator()(Fun T::* t) const volatile noexcept { return t; }

	template <typename F, typename  T>
	constexpr F T::* operator()(F T::* t) const volatile noexcept { return t; }
};

template <typename R>
struct Overload<R()> : public Overload<R() const>
{
	using Fun = R();
	using Overload<R() const>::operator();

	template <typename T>
	constexpr Fun T::* operator()(Fun T::* t) const noexcept { return t; }

	constexpr Fun* operator()(Fun* t) const noexcept { return t; }

	template <typename F>
	constexpr F* operator()(F* t) const noexcept { return t; }
};

/*
 * 这个类的目的是让这部分代码复用，跟语法相关，没有其他含义
 */
template <typename R, typename... As>
struct Base: public Overload<R(As...) const>
{
	using Fun = R(As...);
	using Overload<R(As...) const>::operator();

	template <typename T>
	constexpr Fun T::* operator()(Fun T::* t) const noexcept { return t; }

	constexpr Fun* operator()(Fun* t) const noexcept { return t; }

	template <typename F>
	constexpr F* operator()(F* t) const noexcept { return t; }
};

template <typename R, typename... As>
struct Overload<R(As...)> : public Base<R, As...>
{
};
} // namespace imp

template <class Sig>
constexpr impl::Overload<Sig> const Signature = {};

} // namespace common
