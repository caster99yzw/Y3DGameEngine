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
	using ParamterType = std::tuple<ParamTs...>;
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

template <typename FuncType, std::size_t Index>
using FuncParameterType = std::tuple_element_t<Index, typename FunctionTraits<FuncType>::ParamterType>;

}

