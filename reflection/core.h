#pragma once
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include <xtr1common>
#include <tuple>

namespace common
{

template<typename T>
struct MoveWrapper
{
	MoveWrapper(T &&value) : value(std::move(value)) {}

	MoveWrapper(MoveWrapper const &other) : value(std::move(other.value)) {}
	MoveWrapper &operator=(MoveWrapper const &other)
	{
		value = std::move(other.value);
		return *this;
	}

	MoveWrapper(MoveWrapper &&other) : value(std::move(other.value)) {}
	MoveWrapper &operator=(MoveWrapper &&other)
	{
		value = std::move(other.value);
		return *this;
	}

	mutable T value;
};

template<typename T>
static MoveWrapper<T> MakeRref(T&& value)
{
	return { std::move(value) };
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename T>
static std::shared_ptr<T> CreateIfEmpty(const std::shared_ptr<T>& obj)
{
	return (obj.get() ? obj : std::make_shared<T>());
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename T, T... Ns>
struct IntegerSequence 
{
	template <T N>
	using Append = IntegerSequence<T, Ns..., N>;

	using Type = T;
	using Next = Append<sizeof...(Ns)>;
};

template <typename T, T index, std::size_t N>
struct GenerateSequence
{
	using Type = typename GenerateSequence<T, index - 1, N - 1>::Type::Next;
};

template <typename T, T index>
struct GenerateSequence<T, index, 0ul>
{
	using Type = IntegerSequence<T>;
};

template <std::size_t N>
using MakeIndexSequence = typename GenerateSequence<std::size_t, N, N>::Type;

template <std::size_t... Ns>
using IndexSequence = IntegerSequence<std::size_t, Ns...>;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename... T>
struct type_list {};

template <typename lhsT, typename rhsT>
struct IsSameListSize;

template <typename... lhsT, typename... rhsT>
struct IsSameListSize<type_list<lhsT...>, type_list<rhsT...>>
	: std::integral_constant<bool, sizeof...(lhsT) == sizeof...(rhsT)>
{};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

}

