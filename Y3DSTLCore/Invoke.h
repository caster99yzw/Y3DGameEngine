#pragma once





/**
* Invokes a callable with a set of arguments.  Allows the following:
*
* - Calling a functor object given a set of arguments.
*
**/
template <typename FuncType, typename... ArgTypes>
FORCEINLINE auto Invoke(FuncType&& Func, ArgTypes&&... Args)
	-> decltype(std::forward(FuncType)(Func)(std::forward(ArgTypes)(Args)...))
{
	return std::forward(FuncType)(Func)(std::forward(ArgTypes)(Args)...);
}

