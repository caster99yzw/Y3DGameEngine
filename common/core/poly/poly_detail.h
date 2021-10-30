#pragma once
#include <functional>
#include <tuple>
#include "common/core/type_traits.h"
#include "common/core/aligned_type.h"

namespace common {

template <typename T>
T& StaticConst() { static T value{}; return value; }

template <typename T, template <typename...> typename U>
struct IsInstanceOf : std::false_type {};
template <typename... Ts, template <typename...> typename U>
struct IsInstanceOf<U<Ts...>, U> : std::true_type {};

template <typename L, typename R>
struct XOR : std::false_type {};
template <>
struct XOR<std::true_type, std::false_type> : std::true_type {};
template <>
struct XOR<std::false_type, std::true_type> : std::true_type {};

template <bool b>
using Bool = std::bool_constant<b>;
template <typename Boolean>
using Negation = std::bool_constant<!Boolean::value>;
	
template <typename P, typename Q>
struct MetaAddRef 
{
	template <typename Ts>
	using Apply = MetaApply<P, MetaApply<Q, Ts>>;
};

template <typename T>
struct XRefImpl : MetaIdentity {};
template <typename T>
using XRef = XRefImpl<T>;
template <typename T>
struct XRefImpl<T&> : MetaAddRef<MetaQuote<std::add_lvalue_reference_t>, XRef<T>> {};
template <typename T>
struct XRefImpl<T&&> : MetaAddRef<MetaQuote<std::add_rvalue_reference_t>, XRef<T>> {};
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
template <typename R, typename This, typename... As>
struct IsConstMember<R(*)(const This&, As...)> : std::true_type {};

template <typename Then>
decltype(auto) if_constexpr(std::true_type, Then then) { return then(); }
template <typename Then>
void if_constexpr(std::false_type, Then) {}

template <typename Then, typename Else>
decltype(auto) if_constexpr(std::true_type, Then then, Else) { return then(); }
template <typename Then, typename Else>
decltype(auto) if_constexpr(std::false_type, Then, Else else_) { return else_(); }

template <auto...>
struct PolyMembers;

template <typename I>
struct Poly;
template <typename I>
struct PolyVal;
template <typename I>
struct PolyRef;

namespace impl {

struct Data;
struct PolyAccess;

template <typename T>
using IsPoly = IsInstanceOf<remove_cvref_t<T>, Poly>;

template <typename Member>
struct SignatureOf;

template <typename R, typename C, typename... As>
struct SignatureOf<R(C::*)(As...)>
{
	using type = R(*)(Data&, As...);
};

template <typename R, typename C, typename... As>
struct SignatureOf<R(C::*)(As...) noexcept>
{
	using type = R(*)(Data&, As...);
};

template <typename R, typename C, typename... As>
struct SignatureOf<R(C::*)(As...) const>
{
	using type = R(*)(const Data&, As...);
};

template <typename R, typename C, typename... As>
struct SignatureOf<R(C::*)(As...) const noexcept>
{
	using type = R(*)(const Data&, As...);
};

template <typename R, typename This, typename... As>
struct SignatureOf<R(*)(This&, As...)>
{
	using type = R(*)(Data&, As...);
};

template <typename R, typename This, typename... As>
struct SignatureOf<R(*)(This const&, As...)>
{
	using type = R(*)(const Data&, As...);
};

template <auto Arch>
using SignatureOf_t = _t<SignatureOf<decltype(Arch)>>;

template <auto User, typename Sig = SignatureOf_t<User>>
struct ArgTypes;

template <auto User, typename Ret, typename Data, typename... Args>
struct ArgTypes<User, Ret(*)(Data, Args...)>
{
	using type = TypeList<Args...>;
};

template <auto User>
using ArgTypes_t = _t<ArgTypes<User>>;

template <typename R, typename... Args>
using FnPtr = R(*)(Args...);

struct PolyBase {};
enum class State : short { Empty, InSitu, OnHeap };
enum class Op : short { Nuke, Move, Copy, Type, Addr, Refr };

template <typename I, typename T>
using MembersOf = typename I::template Members<remove_cvref_t<T>>;

template <typename I, typename T>
using InterfaceOf = typename I::template Interface<T>;

template <typename I, typename = void>
struct SubsumptionsOf
{
	using type = TypeList<>;
};

template <typename I>
using InclusiveSubsumptionsOf = TypePushFront<_t<SubsumptionsOf<I>>, I>;

template <typename I>
struct SubsumptionsOf<I, std::void_t<typename I::Subsumptions>>
{
	using type = TypeJoin<TypeTransform<
		typename I::Subsumptions,
		MetaQuote<InclusiveSubsumptionsOf>
		>>;
};

template <typename I>
using SubsumptionsOf_t = _t<SubsumptionsOf<I>>;

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

template <typename I>
struct ArchetypeRoot : ArchetypeBase
{};

template <typename I>
using Archetype = TypeFold<
	InclusiveSubsumptionsOf<I>,
	ArchetypeRoot<I>,
	MetaQuote<InterfaceOf>>;

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

template <typename T>
FORCEINLINE constexpr bool IsInSitu()
{
	return !std::is_reference<T>::value
		&& sizeof(std::decay_t<T>) <= sizeof(Data)
		&& std::is_nothrow_move_constructible<std::decay_t<T>>::value;
}

template <typename T>
FORCEINLINE T& GetInsClass(Data& d)
{
	if (IsInSitu<T>()) {
		return *std::add_pointer_t<T>(
			static_cast<void*>(&(d.Buffer)));
	}
	else {
		return *static_cast<std::add_pointer_t<T>>(d.Object);
	}
}

template <typename T>
FORCEINLINE const T& GetInsClass(const Data& d)
{
	if (IsInSitu<T>()) {
		return *std::add_pointer_t<const T>(
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
		break;
	case Op::Refr:
		break;
	}
	return nullptr;
}

template <typename I, typename T,
	std::enable_if_t<Negation<std::is_reference<T>>::value, int> = 0
>
void* ExecOnHeap(Op op, Data* from, void* to)
{
	switch (op) {
	case Op::Nuke:
		delete &GetInsClass<T>(*from);
		break;
	case Op::Move:
		if_constexpr(std::is_move_constructible<T>(),
			[&]() {
				static_cast<Data*>(to)->Object =
					std::exchange(from->Object, nullptr);
			});
		break;
	case Op::Copy:
		if_constexpr(std::is_copy_constructible<T>(),
			[&]() {
				static_cast<Data*>(to)->Object =
					new T(GetInsClass<T>(*from));
			});
		break;
	case Op::Type:
		return const_cast<void*>(static_cast<void const*>(&typeid(T)));
	case Op::Addr:
		if (*static_cast<std::type_info const*>(to) == typeid(T)) {
			return from->Object;
		}
		break;
	case Op::Refr:
		break;
	}
	return nullptr;
}

template <typename I, typename T,
	std::enable_if_t<std::is_reference<T>::value, int> = 0
>
void* ExecOnHeap(Op op, Data* from, void* to)
{
	switch (op) {
	case Op::Nuke:
		break;
	case Op::Move:
	case Op::Copy:
		static_cast<Data*>(to)->Object = from->Object;
		break;
	case Op::Type:
		return const_cast<void*>(static_cast<void const*>(&typeid(T)));
	case Op::Addr:
		if (*static_cast<std::type_info const*>(to) == typeid(T)) {
			return from->Object;
		}
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
	typename = ArgTypes_t<User>,
	typename = void
>
struct ThunkFn
{
	template <typename R, typename D, typename... As>
	FORCEINLINE constexpr operator FnPtr<R, D&, As...>() const
	{
		return nullptr;
	}
};

template <typename T, auto User, typename... Args>
struct ThunkFn<T, User, TypeList<Args...>,
	std::enable_if_t<!std::is_const<std::remove_reference_t<T>>::value ||
	IsConstMember<decltype(User)>::value>>
{
	template <typename F, typename... As>
	FORCEINLINE static constexpr auto Invoke(F&& f, As&&... as)
		-> decltype(static_cast<F&&>(f)(static_cast<As&&>(as)...))
	{
		return static_cast<F&&>(f)(static_cast<As&&>(as)...);
	}

	template <typename M, typename C, typename... As>
	FORCEINLINE static constexpr auto Invoke(M C::*f, As&&... as)
		-> decltype(std::mem_fn(f)(static_cast<As&&>(as)...))
	{
		return std::mem_fn(f)(static_cast<As&&>(as)...);
	}

	template <typename R, typename D, typename... As>
	FORCEINLINE constexpr operator FnPtr<R, D&, As...>() const
	{
		struct Invoker
		{
			static R Call(D& d, As... as)
			{
				auto& cls = GetInsClass<T>(d);
				return std::invoke(User, cls, std::forward<As>(as)...);
			}
		};
		return &Invoker::Call;
	}
};

template <typename T, auto User>
FORCEINLINE constexpr ThunkFn<T, User> Thunk()
{
	return ThunkFn<T, User>{};
}

template <
	typename I,
	typename Members = MembersOf<I, Archetype<I>>,
	typename Bases = SubsumptionsOf_t<I>
>
struct VTable;

template <typename I, typename T>
struct VTableFor : VTable<I>
{
	constexpr VTableFor()
		: VTable<I>{ Type<T>{}, MembersOf<I, T>{} } {}
};

template <typename I>
FORCEINLINE constexpr VTable<I> const* GetVtable()
{
	return &StaticConst<VTable<I>>();
}

template <typename I, typename T>
FORCEINLINE constexpr VTable<I> const* GetVtable()
{
	return &StaticConst<VTableFor<I, T>>();
}

template <typename I>
struct BasePtr
{
	const VTable<I>* m_vptr = nullptr;
};

template <typename I, auto... Arch, typename... S>
struct VTable<I, PolyMembers<Arch...>, TypeList<S...>>
	: BasePtr<S>..., std::tuple<SignatureOf_t<Arch>...>
{
protected:
	template <typename T, auto... User>
	constexpr VTable(Type<T>, PolyMembers<User...>)
		: BasePtr<S>{GetVtable<S, T>()}...
		, std::tuple<SignatureOf_t<Arch>...>{Thunk<T, User>()...}
		, m_state(IsInSitu<T>() ? State::InSitu : State::OnHeap)
		, m_ops(GetOps<I, T>())
	{}

public:
	constexpr VTable()
		: BasePtr<S>{GetVtable<S>()}... {}

	State m_state = State::Empty;
	Exec m_ops = nullptr;
};

template <typename I>
class PolyRoot : private PolyBase, private Data
{
public:
	friend PolyAccess;
	friend Poly<I>;
	friend PolyVal<I>;
	friend PolyRef<I>;
	using PolyInterface = I;

private:
	FORCEINLINE PolyRoot& Root() { return *this; }
	FORCEINLINE PolyRoot const& Root() const { return *this; }
	const VTable<std::decay_t<I>>* m_vptr = GetVtable<std::decay_t<I>>();
};

template <typename I>
FORCEINLINE constexpr VTable<I> const& Select(const VTable<I>& vtbl) noexcept
{
	return vtbl;
}

template <typename I>
FORCEINLINE constexpr VTable<I> const& Select(const BasePtr<I>& base) noexcept
{
	return *base.m_vptr;
}

struct PolyAccess
{
	template <std::size_t N, typename This, typename... As>
	FORCEINLINE static auto call(This&& t, As&&... args)
		-> decltype(static_cast<This&&>(t).template Call<N>(
			static_cast<As&&>(args)...))
	{
		return static_cast<This&&>(t).template Call<N>(
			static_cast<As&&>(args)...);
	}

	template <typename I>
	FORCEINLINE static const VTable<remove_cvref_t<I>>* vtable(const PolyRoot<I>& t)
	{
		return t.m_vptr;
	}

	template <typename I>
	FORCEINLINE static Data* data(PolyRoot<I>& t)
	{
		return &t;
	}

	template <typename I>
	FORCEINLINE static const Data* data(const PolyRoot<I>& t)
	{
		return &t;
	}

	template <class T, class Poly>
	FORCEINLINE static decltype(auto) cast(Poly&& t)
	{
		using Ret = AddCvrefOf<T, Poly&&>;
		auto* data = const_cast<Data*>(static_cast<const Data*>(&t));
		auto* type = const_cast<void*>(static_cast<const void*>(&typeid(T)));
		return static_cast<Ret>(*static_cast<std::add_pointer_t<Ret>>(
			t.m_vptr->m_ops(Op::Addr, data, type)));
	}
};

template <typename I, typename Tail>
struct PolyNode : Tail
{
	using Tail::Tail;

	template <std::size_t K, typename... As>
	FORCEINLINE decltype(auto) Call(As&&... as)
	{
		return std::get<K>(Select<I>(*PolyAccess::vtable(*this)))(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
	template <std::size_t K, typename... As>
	FORCEINLINE decltype(auto) Call(As&&... as) const
	{
		return std::get<K>(Select<I>(*PolyAccess::vtable(*this)))(
			*PolyAccess::data(*this), static_cast<As&&>(as)...);
	}
};

struct MakePolyNode
{
	template <typename I, typename State>
	using Apply = InterfaceOf<I, PolyNode<I, State>>;
};

template <typename I>
using PolyImpl = TypeFold<
	InclusiveSubsumptionsOf<remove_cvref_t<I>>,
	PolyRoot<I>,
	MakePolyNode>;

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

template <typename T, typename I>
struct InterfaceMatching<
	T,
	I,
	std::enable_if_t<
	Negation<std::is_base_of<PolyBase, std::decay_t<T>>>::value
	>> : InterfaceMatchingImpl<T, I> {};

} // namespace impl 
} // namespace common
