#pragma once
#include "reflection/core.h"
#include "function_traits.h"

namespace reflection
{

namespace detail
{
struct VoidMemberFunc {};
struct DefaultMemberFunc {};
}

class Variant;
class Class;

template <typename FuncType>
using InvokerType = std::conditional_t<HasReturnType<FuncType>::value, 
									   detail::DefaultMemberFunc,
									   detail::VoidMemberFunc>;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename FuncType, typename InvokerT, typename Indices>
struct MethodInvoker;


template <typename FuncType, std::size_t... Index>
struct MethodInvoker<FuncType, detail::DefaultMemberFunc, common::IndexSequence<Index...>>
{
	using ClassT = typename FunctionTraits<FuncType>::ClassType;

	template <typename... ArgTs>
	static Variant Invoke(FuncType func, Variant &obj, ArgTs &&... arguments)
	{
		ClassT *cls = obj.ValuePtr<ClassT>();
		return (cls->*func)(arguments. template Value<FuncParameterType<FuncType, Index>>()...);
	};
};

template <typename FuncType, std::size_t... Index>
struct MethodInvoker<FuncType, detail::VoidMemberFunc, common::IndexSequence<Index...>>
{
	using ClassT = typename FunctionTraits<FuncType>::ClassType;

	template <typename... ArgTs>
	static Variant Invoke(FuncType func, Variant &obj, ArgTs &&... arguments)
	{
		ClassT *cls = obj.ValuePtr<ClassT>();
		(cls->*func)(arguments. template Value<FuncParameterType<FuncType, Index>>()...);
		return Variant();
	};
};

}

