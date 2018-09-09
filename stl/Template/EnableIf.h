#pragma once

/**
* Includes a function in an overload set if the predicate is true.
* 
* template <typename T>
* typename TEnableIf<SomeTrait<T>::Value, ReturnType>::Type Function(const T& Obj)
* {
*     ...
* }
*
**/
template <bool Predicate, typename Result = void>
class EnableIf;

template <typename Result>
class EnableIf<true, Result>
{
public:
	typedef Result Type;
};

template <typename Result>
class EnableIf<false, Result>
{ };

/**
* This is a variant of the above that will determine the return type 'lazily', i.e. only if the function is enabled.
*
* template <typename T>
* typename TLazyEnableIf<SomeTrait<T>::Value, Transform<T>>::Type Function(const T& Obj)
* {
*     ...
* }
*
**/
template <bool Predicate, typename Func>
class LazyEnableIf;

template <typename Func>
class LazyEnableIf<true, Func>
{
public:
	typedef typename Func::Type Type;
};

template <typename Func>
class LazyEnableIf<false, Func>
{ };