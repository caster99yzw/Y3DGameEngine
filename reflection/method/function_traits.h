#pragma once
#include "reflection/core.h"

namespace reflection
{

template <typename FuncType>
struct FunctionTraits : FunctionTraits<decltype(&FuncType::operator())> {};

template <typename RetT, typename... ParamTs>
struct FunctionTraits<RetT (ParamTs...)>
{
	constexpr static std::size_t ParameterCount = sizeof...(ParamTs);
	using ReturnType = RetT;
	using ParamterType = common::TypeList<ParamTs...>;
};

template <typename T>
struct FunctionTraits<std::function<T>> : FunctionTraits<T> {};

template <typename RetT, typename ClassT, typename... ParamTs>
struct FunctionTraits<RetT(ClassT::*)(ParamTs...)> : FunctionTraits<RetT (ParamTs...)>
{
	using ClassType = ClassT;
};

template <typename RetT, typename ClassT, typename... ParamTs>
struct FunctionTraits<RetT(ClassT::*)(ParamTs...) const> : FunctionTraits<RetT (ParamTs...)>
{
	using ClassType = ClassT;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename FuncType, typename = typename FunctionTraits<FuncType>::ClassType>
struct HasClassType : std::true_type
{
};

template <typename FuncType>
struct HasClassType<FuncType, void> : std::false_type
{
};

template <typename FuncType, typename = typename FunctionTraits<FuncType>::ReturnType>
struct HasReturnType : std::true_type
{
};

template <typename FuncType>
struct HasReturnType<FuncType, void> : std::false_type
{
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <std::size_t Index, typename ParamT>
struct IndexedParameter {};


template <typename ParamList, std::size_t Index>
struct IndexedParameterType;

template <typename... ParamTs, std::size_t Index>
struct IndexedParameterType<common::TypeList<ParamTs...>, Index>
{
private:
	template <typename Indices>
	struct ParameterSet;

	template <std::size_t... Is>
	struct ParameterSet<common::IndexSequence<Is...>> : IndexedParameter<Is, ParamTs>... {};

	template <typename T>
	inline static std::enable_if_t<true, T> Impl(IndexedParameter<Index, T>);
	inline static std::enable_if<false> Impl(...);

public:
	using Type = decltype(Impl(ParameterSet<common::MakeIndexSequence<sizeof...(ParamTs)>>()));
};

template <typename FuncType, std::size_t Index>
struct FuncParameterTypeImpl
{
	using ParamList = typename FunctionTraits<FuncType>::ParamterType;
	using Type = typename IndexedParameterType<ParamList, Index>::Type;
};

template <typename FuncType, std::size_t Index>
using FuncParameterType = typename FuncParameterTypeImpl<FuncType, Index>::Type;

}

