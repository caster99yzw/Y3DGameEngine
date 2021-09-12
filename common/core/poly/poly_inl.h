#pragma once
#include <tuple>
#include "common/core/overload.h"
#include "common/core/type_traits.h"
#include "common/core/aligned_type.h"

namespace common {

template <typename T>
struct StaticConst
{
	static constexpr T value{};
};

template <typename Fun>
struct IsConstMember : std::false_type {};

template <typename R, typename C, typename... As>
struct IsConstMember<R(C::*)(As...) const> : std::true_type {};

template <typename R, typename C, typename... As>
struct IsConstMember<R(C::*)(As...) const noexcept> : std::true_type {};

template <auto...>
struct PolyMembers;

namespace impl {

template <typename I>
struct Poly;
template <typename I>
struct PolyVal;
struct Data;
struct PolyAccess;

struct PolyBase {};
enum class State : short { Empty, InSitu, OnHeap };
enum class Op : short { Nuke, Move, Copy, Type, Addr, Refr };

template <typename I, typename T>
using MembersOf = typename I::template Members<remove_cvref_t<T>>;

template <typename I, typename T>
using InterfaceOf = typename I::template Interface<T>;

struct Data
{
	Data() = default;
	Data(const Data&) {}
	Data& operator=(const Data&) { return *this; }
	union {
		void* Object;
		TypeAlignedBytes<double[2]> Buffer;
	};
};

template <typename...>
struct VTable;

using Exec = void* (*)(Op, Data*, void*);

template <class I, class T>
void* execInSitu(Op op, Data* from, void* to) {
	switch (op) {
	case Op::Nuke:
		break;
	case Op::Move:
		break;
	case Op::Copy:
		break;
	case Op::Type:
		break;
	case Op::Addr:
		break;
	case Op::Refr:
		break;
	}
	return nullptr;
}

template <class I, class T>
constexpr Exec getOps() noexcept {
	return &execInSitu<I, T>;
}

template <class I, auto... Arch>
struct VTable<I, PolyMembers<Arch...>> : std::tuple<decltype(Arch)...>
{
private:
	template <class T, auto... User>
	constexpr VTable(T, PolyMembers<User...>)
		: std::tuple<decltype(Arch)...> {}
		, m_state{ State::InSitu }
		, m_ops{ getOps<I, T>() } {}

public:
	constexpr VTable()
		: std::tuple<decltype(Arch)...>{}
		, m_state{ State::Empty }
		, m_ops{ nullptr } {}

	template <typename T>
	explicit constexpr VTable(T)
		: VTable{ T{}, MembersOf<I, T>() } {}

	State	m_state;
	Exec	m_ops;
};

template <typename I>
constexpr VTable<I> const* vtable()
{
	return &StaticConst<VTable<I>>::value;
}

template <typename I>
class PolyRoot : private PolyBase, private Data
{
public:
	friend PolyAccess;
	friend Poly<I>;
	friend PolyVal<I>;
	using PolyInterface = I;

protected:
	PolyRoot& Root() noexcept { return *this; }
	PolyRoot const& Root() const noexcept { return *this; }
	const VTable<std::decay_t<I>>* m_vptr = vtable<std::decay_t<I>>();
};

struct PolyAccess
{
	template <std::size_t N, typename This, typename... As>
	static auto call(This&& t, As&&... args)
		-> decltype(static_cast<This&&>(t).template Call<N>(
			static_cast<As&&>(args)...))
	{
		return static_cast<This&&>(t).template Call<N>(
			static_cast<As&&>(args)...);
	}

	template <class I>
	static const VTable<remove_cvref_t<I>>* vtable(const PolyRoot<I>& t)
	{
		return t.m_vptr;
	}

	template <class I>
	static Data* data(PolyRoot<I>& t)
	{
		return &t;
	}

	template <class I>
	static const Data* data(const PolyRoot<I>& t)
	{
		return &t;
	}
};

template <class I>
struct PolyNode : public PolyRoot<I> {
	template <std::size_t K, typename... As>
	decltype(auto) Call(As&&... as)
	{
		return std::get<K>(*PolyAccess::vtable(*this))(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
	template <std::size_t K, typename... As>
	decltype(auto) Call(As&&... as) const
	{
		return std::get<K>(*PolyAccess::vtable(*this))(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
};

template <class I>
using PolyImpl = InterfaceOf<I, PolyNode<I>>;

} // namespace impl 
} // namespace common
