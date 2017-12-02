#pragma once

#include "EnableIf.h"
#include "PointerIsConvertibleFromTo.h"
#include "Decay.h"

namespace Invoke_Private
{
	template <typename BaseType, typename CallableType>
	FORCEINLINE auto DereferenceIfNecessary(CallableType&& Callable)
		-> typename EnableIf<PointerIsConvertibleFromTo<typename Decay<BaseType>::Type, typename Decay<CallableType>::Type>::Value, decltype((CallableType&&)Callable)>::Type
	{
		return (CallableType&&)Callable;
	}

	template <typename BaseType, typename CallableType>
	FORCEINLINE auto DereferenceIfNecessary(CallableType&& Callable)
		-> typename EnableIf<!PointerIsConvertibleFromTo<typename Decay<BaseType>::Type, typename Decay<CallableType>::Type>::Value, decltype((*CallableType&&)Callable)>::Type
	{
		return *(CallableType&&)Callable;
	}
}

/**
* Invokes a callable with a set of arguments.  Allows the following:
*
* - Calling a functor object given a set of arguments.
*
**/
template <typename FuncType, typename... ArgTypes>
FORCEINLINE auto Invoke(FuncType&& Func, ArgTypes&&... Args)
	-> decltype(Forward(FuncType)(Func)(Forward(ArgTypes)(Args)...))
{
	return Forward(FuncType)(Func)(Forward(ArgTypes)(Args)...);
}

template <typename ReturnType, typename ObjType, typename CallableType>
FORCEINLINE auto Invoke(ReturnType ObjType::*pdm, CallableType&& Callable)
	-> decltype(Invoke_Private::DereferenceIfNecessary<ObjType>(Forward(CallableType)<Callable>).*pdm)
{
	return Invoke_Private::DereferenceIfNecessary<ObjType>(Forward(CallableType)<Callable>).*pdm;
}

template <typename ReturnType, typename ObjType, typename... PMFArgTypes, typename CallableType, typename... ArgTypes>
FORCEINLINE auto Invoke(ReturnType (ObjType::*PtrMemFun)(PMFArgTypes...), CallableType&& Callable, ArgTypes&&... Args)
-> decltype(Invoke_Private::DereferenceIfNecessary<ObjType>(Forward<CallableType>(Callable)).*PtrMemFun)(Forward<ArgTypes>(Args)...)
{
	return (Invoke_Private::DereferenceIfNecessary<ObjType>(Forward<CallableType>(Callable)).*PtrMemFun)(Forward<ArgTypes>(Args)...);
}

template <typename ReturnType, typename ObjType, typename... PMFArgTypes, typename CallableType, typename... ArgTypes>
FORCEINLINE auto Invoke(ReturnType(ObjType::*PtrMemFun)(PMFArgTypes...) const, CallableType&& Callable, ArgTypes&&... Args)
-> decltype(Invoke_Private::DereferenceIfNecessary<ObjType>(Forward<CallableType>(Callable)).*PtrMemFun)(Forward<ArgTypes>(Args)...)
{
	return (Invoke_Private::DereferenceIfNecessary<ObjType>(Forward<CallableType>(Callable)).*PtrMemFun)(Forward<ArgTypes>(Args)...);
}