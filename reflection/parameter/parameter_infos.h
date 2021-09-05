#pragma once
#include "reflection/core.h"
#include "reflection/method/function_traits.h"

namespace reflection
{

namespace detail
{
struct FunctionParams {};
struct ConstructorParams {};
}

template <typename... ParamWrapperTs>
struct ParameterInfos
{
	constexpr static std::size_t size() { return sizeof...(ParamWrapperTs); }
	std::tuple<ParamWrapperTs...> parameters;
};

template <>
struct ParameterInfos<>
{
	constexpr static std::size_t size() { return 0; }
	std::tuple<> parameters;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename NamePolicy, typename ParamList, typename Indices>
struct GenerateParameterInfosImpl;

template <typename NamePolicy, typename... ParamTs, std::size_t... Index>
struct GenerateParameterInfosImpl<NamePolicy, common::type_list<ParamTs...>, common::IndexSequence<Index...>>
{
	using Type = ParameterInfos<ParameterWrapper<NamePolicy, ParamTs, Index>...>;
};

template <typename NamePolicy, typename FuncType, std::size_t... Index>
struct GenerateParameterInfosImpl<NamePolicy, FuncType, common::IndexSequence<Index...>>
{
using Type = ParameterInfos<ParameterWrapper<NamePolicy, FuncParameterTypeImpl<FuncType, Index>, Index>...>;
};


template <typename FuncType, typename NamePolicy>
struct GenerateParameterInfos
{
	using Type = typename GenerateParameterInfosImpl<NamePolicy, FuncType, 
		common::MakeIndexSequence<FunctionTraits<FuncType>::ParameterCount>>::Type;
};

template <typename NamePolicy, typename... ParamTs>
struct GenerateParameterInfos<common::type_list<ParamTs...>, NamePolicy>
{
	using Type = typename GenerateParameterInfosImpl<NamePolicy, 
		common::type_list<ParamTs...>, 
		common::MakeIndexSequence<sizeof...(ParamTs)>>::Type;
};

template <typename T, typename NamePolicy>
using MakeParameterInfos = typename GenerateParameterInfos<T, NamePolicy>::Type;

}

