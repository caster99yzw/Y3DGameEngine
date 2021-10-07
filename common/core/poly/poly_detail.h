#pragma once
#include <functional>
#include <tuple>
#include "common/core/overload.h"
#include "common/core/type_traits.h"
#include "common/core/aligned_type.h"

namespace common {

template <typename T>
T& StaticConst() { static T value{}; return value; }

template <bool b>
using Bool = std::bool_constant<b>;
	
template <typename T>
struct XRefImpl : MetaIdentity {};
template <typename T>
using XRef = XRefImpl<T>;
template <typename T>
struct XRefImpl<T&> : MetaCompose<MetaQuote<std::add_lvalue_reference_t>, XRef<T>> {};
template <typename T>
struct XRefImpl<T&&> : MetaCompose<MetaQuote<std::add_rvalue_reference_t>, XRef<T>> {};
template <typename T>
struct XRefImpl<T const> : MetaQuote<std::add_const_t> {};

template <typename A, typename B>
using AddCvrefOf = MetaApply<XRef<B>, A>;

template <typename Fun>
struct IsConstMember : std::false_type {};
template <typename R, typename C, typename... As>
struct IsConstMember<R(C::*)(As...) const> : std::true_type {};
template <typename R, typename C, typename... As>
struct IsConstMember<R(C::*)(As...) const noexcept> : std::true_type {};

template <class Then>
decltype(auto) if_constexpr(std::true_type, Then then) { return then(); }
template <class Then>
void if_constexpr(std::false_type, Then) {}

template <class Then, class Else>
decltype(auto) if_constexpr(std::true_type, Then then, Else) { return then(); }
template <class Then, class Else>
decltype(auto) if_constexpr(std::false_type, Then, Else else_) { return else_(); }

template <auto...>
struct PolyMembers;

namespace impl {

template <typename I>
struct Poly;
template <typename I>
struct PolyVal;
struct Data;
struct PolyAccess;

template <class Member>
struct SignatureOf;

template <class R, class C, class... As>
struct SignatureOf<R(C::*)(As...)>
{
	using type = R(*)(Data&, As...);
};

template <class R, class C, class... As>
struct SignatureOf<R(C::*)(As...) const>
{
	using type = R(*)(const Data&, As...);
};

template <auto Arch>
using SignatureOf_t = _t<SignatureOf<decltype(Arch)>>;

template <auto User, class Sig = SignatureOf_t<User>>
struct ArgTypes;

template <auto User, class Ret, class Data, class... Args>
struct ArgTypes<User, Ret(*)(Data, Args...)>
{
	using type = TypeList<Args...>;
};

template <auto User>
using ArgTypes_t = _t<ArgTypes<User>>;

template <class R, class... Args>
using FnPtr = R(*)(Args...);

struct PolyBase {};
enum class State : short { Empty, InSitu, OnHeap };
enum class Op : short { Nuke, Move, Copy, Type, Addr, Refr };

template <typename I, typename T>
using MembersOf = typename I::template Members<remove_cvref_t<T>>;

template <typename I, typename T>
using InterfaceOf = typename I::template Interface<T>;

struct Bottom
{
	template <typename T>
	[[noreturn]] operator T && () const
	{
		std::terminate();
	}
};

struct ArchetypeBase : Bottom
{
	template <std::size_t, typename... As>
	[[noreturn]] Bottom Call(As&&...) const
	{
		std::terminate();
	}
};

struct Data
{
	Data() = default;
	Data(const Data&) {}
	Data& operator=(const Data&) { return *this; }
	union
	{
		void* Object{};
		TypeAlignedBytes<double[2]> Buffer;
	};
};

template <class T>
FORCEINLINE constexpr bool IsInSitu()
{
	return !std::is_reference<T>::value
		&& sizeof(std::decay_t<T>) <= sizeof(Data)
		&& std::is_nothrow_move_constructible<std::decay_t<T>>::value;
}

template <class T>
T& GetInsClass(Data& d)
{
	if (IsInSitu<T>()) {
		return *static_cast<std::add_pointer_t<T>>(
			static_cast<void*>(&(d.Buffer)));
	}
	else {
		return *static_cast<std::add_pointer_t<T>>(d.Object);
	}
}

template <class T>
const T& GetInsClass(const Data& d)
{
	if (IsInSitu<T>()) {
		return *static_cast<std::add_pointer_t<const T>>(
			static_cast<const void*>(&(d.Buffer)));
	}
	else {
		return *static_cast<std::add_pointer_t<const T>>(d.Object);
	}
}

using Exec = void* (*)(Op, Data*, void*);

template <typename I, typename T>
void* ExecInSitu(Op op, Data* from, void* to)
{
	switch (op) {
	case Op::Nuke:
		GetInsClass<T>(*from).~T();
		break;
	case Op::Move:
		if_constexpr(std::is_move_constructible<T>(),
			[&]() {
				auto* ptr = static_cast<void*>(&(static_cast<Data*>(to)->Buffer));
				new(ptr) T(std::move(GetInsClass<T>(*from)));
				GetInsClass<T>(*from).~T();
			});
		break;
	case Op::Copy:
		if_constexpr(std::is_copy_constructible<T>(),
			[&]() {
				new(static_cast<void*>(&(static_cast<Data*>(to)->Buffer)))
					T(GetInsClass<T>(*from)); }
		);
		break;
	case Op::Type:
		return const_cast<void*>(static_cast<void const*>(&typeid(T)));
	case Op::Addr:
		if (*static_cast<std::type_info const*>(to) == typeid(T)) {
			return &from->Buffer;
		}
		return nullptr;
	case Op::Refr:
		break;
	}
	return nullptr;
}

template <typename I, typename T>
void* ExecOnHeap(Op op, Data* from, void* to)
{
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

template <typename I, typename T>
FORCEINLINE constexpr Exec GetOpsImpl(std::true_type)
{
	return &ExecInSitu<I, T>;
}

template <typename I, typename T>
FORCEINLINE constexpr Exec GetOpsImpl(std::false_type)
{
	return &ExecOnHeap<I, T>;
}

template <typename I, typename T>
FORCEINLINE constexpr Exec GetOps()
{
	return GetOpsImpl<I, T>(Bool<IsInSitu<T>()>{});
}

template <
	typename T,
	auto User,
	typename = ArgTypes_t<User>
>
struct ThunkFn;

template <typename T, auto User, typename... Args>
struct ThunkFn<T, User, TypeList<Args...>>
{
	template <class R, class D, class... As>
	constexpr operator FnPtr<R, D&, As...>() const
	{
		struct _impl
		{
			static R Call(D& d, As... as)
			{
				auto& cls = GetInsClass<T>(d);
				return (cls.*User)(std::forward<As>(as)...);
			}
		};
		return &_impl::Call;
	}
};

template <typename T, auto User>
FORCEINLINE constexpr ThunkFn<T, User> Thunk()
{
	return ThunkFn<T, User>{};
}

template <
	typename I,
	typename Members = MembersOf<I, InterfaceOf<I, ArchetypeBase>>
>
struct VTable;

template <typename I, auto... Arch>
struct VTable<I, PolyMembers<Arch...>>
{
	explicit VTable() = default;

	VTable(std::tuple<SignatureOf_t<Arch>...>&& vtbl, State state, Exec ops)
		: m_vtbl(std::move(vtbl))
		, m_state(state)
		, m_ops(ops)
	{}

	std::tuple<SignatureOf_t<Arch>...> m_vtbl;
	State m_state = State::Empty;
	Exec m_ops = nullptr;
};

template <typename I, typename T, auto... User>
FORCEINLINE VTable<I> CreateVTableFor(PolyMembers<User...>)
{
	return {
		std::make_tuple(Thunk<T, User>()...),
		IsInSitu<T>() ? State::InSitu : State::OnHeap,
		GetOps<I, T>()
	};
}

template <typename I, typename T>
struct VTableFor : VTable<I>
{
	 VTableFor()
		: VTable<I>{ CreateVTableFor<I, T>(MembersOf<I, T>()) } {}
};

template <typename I>
FORCEINLINE VTable<I> const* GetVtable()
{
	return &StaticConst<VTable<I>>();
}

template <typename I, typename T>
FORCEINLINE VTable<I> const* GetVtable()
{
	return &StaticConst<VTableFor<I, T>>();
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
	PolyRoot& Root() { return *this; }
	PolyRoot const& Root() const { return *this; }
	const VTable<std::decay_t<I>>* m_vptr = GetVtable<std::decay_t<I>>();
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

	template <typename I>
	static const VTable<remove_cvref_t<I>>* vtable(const PolyRoot<I>& t)
	{
		return t.m_vptr;
	}

	template <typename I>
	static Data* data(PolyRoot<I>& t)
	{
		return &t;
	}

	template <typename I>
	static const Data* data(const PolyRoot<I>& t)
	{
		return &t;
	}
};

template <typename I>
struct PolyNode : public PolyRoot<I>
{
	template <std::size_t K, typename... As>
	decltype(auto) Call(As&&... as)
	{
		return std::get<K>(PolyAccess::vtable(*this)->m_vtbl)(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
	template <std::size_t K, typename... As>
	decltype(auto) Call(As&&... as) const
	{
		return std::get<K>(PolyAccess::vtable(*this)->m_vtbl)(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
};

template <typename I>
using PolyImpl = InterfaceOf<I, PolyNode<I>>;

template <typename T, typename I, typename = void>
struct InterfaceMatchingImpl : std::false_type {};

template <typename T, typename I>
struct InterfaceMatchingImpl<
	T, I,
	std::void_t<
	std::enable_if_t<
	std::is_constructible_v<AddCvrefOf<std::decay_t<T>, I>, T>,
	MembersOf<std::decay_t<I>, std::decay_t<T>>
	>>> : std::true_type {};

template <typename T, typename I, typename = void>
struct InterfaceMatching : std::false_type {};

template <class T, class I>
struct InterfaceMatching<
	T,
	I,
	std::enable_if_t<
	!std::is_base_of_v<PolyBase, std::decay_t<T>>
	>> : InterfaceMatchingImpl<T, I> {};

} // namespace impl 
} // namespace common
