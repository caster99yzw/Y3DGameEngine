#pragma once

namespace common {

template <typename T>
using _t = typename T::Type;

template <typename Fn, typename... Ts>
using MetaApply = typename Fn::template Apply<Ts...>;

template <typename... Ts>
struct TypeList
{
	static constexpr std::size_t Size = sizeof...(Ts);
	using Type = TypeList;
	template <typename Fn>
	using Apply = MetaApply<Fn, Ts...>;
};

namespace impl
{
template <bool B>
struct IfImpl
{
	template <typename T, typename U>
	using Apply = T;
};
	
template <>
struct IfImpl<false>
{
	template <typename T, typename U>
	using Apply = U;
};
} // namespace impl
	
template <typename P, typename Q>
struct MetaCompose
{
	template <typename... Ts>
	using Apply = MetaApply<P, MetaApply<Q, Ts...>>;
};

struct MetaIdentity
{
	template <typename T>
	using Apply = T;
};

struct MetaQuote
{
	template <typename... Ts>
	using Apply = TypeList<Ts...>;
};

namespace impl {
template <std::size_t N>
struct MetaElementImpl
{
	template <std::size_t N, typename ...Ts>
	struct Lambda; 
	template <std::size_t N, typename A, typename ...Ts>
	struct Lambda<N, A, Ts...>
	{
		template <typename... Us>
		using Apply = MetaApply<Lambda<N - 1, Ts...>>;
	};
	template <typename A, typename ...Ts>
	struct Lambda<0, A, Ts...>
	{
		template <typename... Us>
		using Apply = A;
	};
	template <typename... Ts>
	using Apply = MetaApply<Lambda<N, Ts...>>;
};
} // namespace impl
	
template <std::size_t N>
struct MetaElement
{
	template <typename... Ts>
	using Apply = MetaApply<impl::MetaElementImpl<N>, Ts...>;
};

template <typename List, std::size_t N>
using TypeAt = MetaApply<List, MetaElement<N>>;

template <typename Fn, typename... Ts>
struct MetaBindFront
{
	template <typename... Us>
	using Apply = MetaApply<Fn, Ts..., Us...>;
};

template <typename Fn, typename... Ts>
struct MetaBindBack
{
	template <typename... Us>
	using Apply = MetaApply<Fn, Us..., Ts...>;
};

template <typename List, typename... Ts>
using TypePushBack = MetaApply<List, MetaBindBack<MetaQuote, Ts...>>;

template <typename List, typename... Ts>
using TypePushFront = MetaApply<List, MetaBindFront<MetaQuote, Ts...>>;

namespace impl {
template <typename Fn>
struct MetaTransformImpl
{
	template <typename ...Ts>
	struct Lambda; 
	template <typename A, typename ...Ts>
	struct Lambda<A, Ts...>
	{
		template <typename... Us>
		using Apply = TypePushFront<MetaApply<Lambda<Ts...>>, MetaApply<Fn, A>>;
	};
	template <typename T>
	struct Lambda<T>
	{
		template <typename... Us>
		using Apply = TypePushFront<TypeList<>, MetaApply<Fn, T>>;
	};
	template <typename... Ts>
	using Apply = MetaApply<Lambda<Ts...>>;
};
} // namespace impl

template <typename List, typename Fn>
using TypeTransform = MetaApply<List, impl::MetaTransformImpl<Fn>>;

template <typename Fn>
struct MetaFlip
{
	template <typename L, typename R>
	using Apply = MetaApply<Fn, L, R>;
};

namespace impl {
template <typename Fn>
struct MetaFoldRImpl { template <typename ...Ts>
	struct Lambda : MetaIdentity {};
	template <typename A, typename ...Ts>
	struct Lambda<A, Ts...>
	{
		template <typename State>
		using Apply = MetaApply<Fn, A, MetaApply<Lambda<Ts...>, State>>;
	};
	template <typename... Ts>
	using Apply = Lambda<Ts...>;
};

template <typename Fn>
struct MetaFoldLImpl
{
	template <typename ...Ts>
	struct Lambda : MetaIdentity {};
	template <typename A, typename ...Ts>
	struct Lambda<A, Ts...>
	{
		template <typename State>
		using Apply = MetaApply<Lambda<Ts...>, MetaApply<Fn, State, A>>;
	};
	template <typename... Ts>
	using Apply = Lambda<Ts...>;
};
} // namespace impl

template <class List, class State, class Fn>
using TypeFold = MetaApply<MetaApply<List, impl::MetaFoldRImpl<Fn>>, State>;
	
template <class List, class State, class Fn>
using TypeReverseFold = MetaApply<MetaApply<List, impl::MetaFoldLImpl<Fn>>, State>;
	
namespace impl {
struct MetaJoinImpl
{
	template <typename Fn>
	struct Lambda
	{
		template <typename... Ts>
		using Apply = MetaBindBack<Fn, Ts...>;
	};
	template <typename List, typename Fn>
	using Apply = MetaApply<List, Lambda<Fn>>;
};
} // namespace impl

template <class List>
using TypeJoin = MetaApply<TypeFold<List, MetaQuote, impl::MetaJoinImpl>>;

template <class... Ts>
using TypeConcat = TypeJoin<TypeList<Ts...>>;

} // namespace common
