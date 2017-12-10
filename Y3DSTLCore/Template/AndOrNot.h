#pragma once

/**
* Does a boolean AND of the ::Value static members of each type, but short-circuits if any Type::Value == false.
*/
template <typename ... Types>
struct And;

template <bool LHSValue, typename ... RHS>
struct AndVaule
{
	enum { Value = And<RHS...>::Value };
};

template <typename ... RHS>
struct AndVaule<false, RHS...>
{
	enum { Value = false };
};

template <typename LHS, typename ... RHS>
struct And<LHS, RHS...> : AndVaule<LHS::Value, RHS...>
{
};

template <>
struct And<>
{
	enum { Value = true };
};

/**
* Does a boolean OR of the ::Value static members of each type, but short-circuits if any Type::Value == true.
*/
template <typename ... Types>
struct Or;

template <bool LHSValue, typename ... RHS>
struct OrVaule
{
	enum { Value = Or<RHS...>::Value };
};

template <typename ... RHS>
struct OrVaule<true, RHS...>
{
	enum { Value = true };
};

template <typename LHS, typename ... RHS>
struct Or<LHS, RHS...> : OrVaule<LHS::Value, RHS...>
{
};

template <>
struct Or<>
{
	enum { Value = false };
};

/**
* Does a boolean NOT of the ::Value static members of the type.
*/
template <typename Type>
struct Not
{
	enum { Value = !Type::Value };
};
