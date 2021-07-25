#pragma once

namespace common {

/// <summary>
/// list of value
/// </summary>
template <auto... Value>
struct value_list
{
	using type = value_list;
	static constexpr std::size_t size = sizeof...(Value);
};

template <std::size_t Idx, typename List>
struct value_list_element;

template <std::size_t Idx, auto First, auto... Other>
struct value_list_element<Idx, value_list<First, Other...>>
	: value_list_element<Idx - 1, value_list<Other...>>
{};

template <auto First, auto... Other>
struct value_list_element<0u, value_list<First, Other...>>
{
	static constexpr auto value = First;
};

template <std::size_t Idx, typename List>
constexpr auto value_list_element_v = value_list_element<Idx, List>::value;

template <typename...>
struct value_list_cat;

template <>
struct value_list_cat<>
{
	using type = value_list<>;
};

template <auto... Value>
struct value_list_cat<value_list<Value...>>
{
	using type = value_list<Value...>;
};

template <auto... Lhs, auto... Rhs, typename... Lists>
struct value_list_cat<value_list<Lhs...>, value_list<Rhs...>, Lists...>
{
	using type = typename value_list_cat<value_list<Lhs..., Rhs...>, Lists...>::type;
};

template <typename... Lists>
using value_list_cat_t = typename value_list_cat<Lists...>::type;

/// <summary>
/// list of type
/// </summary>
template <typename... T>
struct type_list
{
	using type = type_list;
	static constexpr std::size_t size = sizeof...(T);
};

template <std::size_t Idx, typename List>
struct type_list_element;

template <std::size_t Idx, typename First, typename... Other>
struct type_list_element<Idx, type_list<First, Other...>>
	: type_list_element<Idx - 1, type_list<Other...>>
{};

template <typename First, typename... Other>
struct type_list_element<0u, type_list<First, Other...>>
{
	using type = First;
};

template <std::size_t Idx, typename List>
using type_list_element_v = typename type_list_element<Idx, List>::type;

template <typename...>
struct type_list_cat;

template <>
struct type_list_cat<>
{
	using type = type_list<>;
};

template <typename... Value>
struct type_list_cat<type_list<Value...>>
{
	using type = type_list<Value...>;
};

template <typename... Lhs, typename... Rhs, typename... Lists>
struct type_list_cat<type_list<Lhs...>, type_list<Rhs...>, Lists...>
{
	using type = typename type_list_cat<type_list<Lhs..., Rhs...>, Lists...>::type;
};

template <typename... Lists>
using type_list_cat_t = typename type_list_cat<Lists...>::type;

} // namespace common
