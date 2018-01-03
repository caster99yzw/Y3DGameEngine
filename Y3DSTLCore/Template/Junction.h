#pragma once


/**
* If any of types are false, return the first false type
* Otherwise, the last trait in types
* Otherwise, if types is empty, false_type
*
*/
template<typename... Types>
struct Conjunction;

template<bool LHSValue, typename LHS, typename... RHS>
struct ConjunctionValue
{
	using Type = LHS;
};

template<typename LHS, typename... RHS>
struct ConjunctionValue<true, LHS, LHSNext, RHS...>
{
	using Type = typename ConjunctionValue<LHSNext::Value, LHSNext, RHS...>::Type;
};

template<typename LHS, typename... RHS>
struct Conjunction<LHS, RHS...> : ConjunctionValue<LHS::Value, LHS, RHS...>
{	
};

template<>
struct Conjunction<>
{
	using Type = void;
};

/**
* If any of types are true, return the first true type
* Otherwise, the last trait in types
* Otherwise, if types is empty, false_type
*
*/
template<class... Types>
struct Disjunction;

template<bool LHSValue, typename LHS, typename... RHS>
struct DisjunctionValue
{
	using Type = LHS;
};

template<typename LHS, typename LHSNext, typename... RHS>
struct DisjunctionValue<false, LHS, LHSNext, RHS...>
{
	using Type = typename DisjunctionValue<LHSNext::Value, LHSNext, RHS...>::Type;
};


template<class LHS,	class... RHS>
struct Disjunction<LHS, RHS...> : DisjunctionValue<LHS::Value, LHS, RHS...>
{	
};

template<>
struct Disjunction<>
{
	using Type = void;
};