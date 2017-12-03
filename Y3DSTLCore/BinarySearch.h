#pragma once

#include "Template/IdentityFunctor.h"
#include "Template/Invoke.h"
#include "Template/Template.h"
#include "Template/Less.h"

namespace AlgoImpl
{
	//////////////////////////////////////////////////////////////////////////
	// Lower Bound algorithm implement

	template <typename RangeValueType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
	FORCEINLINE SIZE_T LowerBoundInternal(RangeValueType* First, const SIZE_T Num, PredicateValueType const& Value, ProjectionType Projection, SortPredicateType SortPredicate)
	{
		SIZE_T start = 0;

		SIZE_T Size = Num;


		while (Size > 0)
		{
			SIZE_T const LeftoverSize = Size % 2;
			Size = Size / 2;

			SIZE_T const CheckIndex = Start + Size;
			SIZE_T const StartIfLess = CheckIndex + LeftoverSize;

			auto&& CheckValue = Invoke(Projection, First[CheckIndex]);
			Start = SortPredicate(CheckValue, Value) ? StartIfLess : Start;
		}
		return start;
	}

	//////////////////////////////////////////////////////////////////////////
	// Upper Bound algorithm implement

	template <typename RangeValueType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
	FORCEINLINE SIZE_T UpperBoundInternal(RangeValueType* First, const SIZE_T Num, PredicateValueType const& Value, ProjectionType Projection, SortPredicateType SortPredicate)
	{
		SIZE_T start = 0;

		SIZE_T Size = Num;

		while (Size > 0)
		{
			SIZE_T const LeftoverSize = Size % 2;
			Size = Size / 2;

			SIZE_T const CheckIndex = Start + Size;
			SIZE_T const StartIfLess = CheckIndex + LeftoverSize;

			auto&& CheckValue = Invoke(Projection, First[CheckIndex]);
			Start = !SortPredicate(CheckValue, Value) ? StartIfLess : Start;
		}
		return start;
	}
};

namespace Algo
{
	//////////////////////////////////////////////////////////////////////////
	// Lower Bound with default projection type

	template <typename RangeType, typename ValueType, typename SortPredicateType>
	FORCEINLINE UINT32 LowerBound(RangeType& Range, ValueType const& Value, SortPredicateType SortPredicate)
	{
		return AlgoImpl::LowerBoundInternal(GetData(Range), GetNum(Range), Value, IdentityFunctor(), SortPredicate);
	}
	template <typename RangeType, typename ValueType>
	FORCEINLINE UINT32 LowerBound(RangeType& Range, ValueType const& Value)
	{
		return AlgoImpl::LowerBoundInternal(GetData(Range), GetNum(Range), Value, IdentityFunctor(), Less<>());
	}

	//////////////////////////////////////////////////////////////////////////
	// Lower Bound with self-define projection type

	template <typename RangeType, typename ValueType, typename ProjectionType, typename SortPredicateType>
	FORCEINLINE UINT32 LowerBoundBy(RangeType& Range, ValueType const& Value, ProjectionType Projection, SortPredicateType SortPredicate)
	{
		return AlgoImpl::LowerBoundInternal(GetData(Range), GetNum(Range), Value, Projection, SortPredicate);
	}
	template <typename RangeType, typename ValueType, typename ProjectionType>
	FORCEINLINE UINT32 LowerBoundBy(RangeType& Range, ValueType const& Value, ProjectionType Projection)
	{
		return AlgoImpl::LowerBoundInternal(GetData(Range), GetNum(Range), Value, Projection, Less<>());
	}

	//////////////////////////////////////////////////////////////////////////
	// Upper Bound with default projection type

	template <typename RangeType, typename ValueType, typename SortPredicateType>
	FORCEINLINE UINT32 UpperBound(RangeType& Range, ValueType const& Value, SortPredicateType SortPredicate)
	{

		return AlgoImpl::UpperBoundInternal(GetData(Range), GetNum(Range), Value, IdentityFunctor(), SortPredicate);
	}
	template <typename RangeType, typename ValueType>
	FORCEINLINE UINT32 UpperBound(RangeType& Range, ValueType const& Value)
	{
		return AlgoImpl::UpperBoundInternal(GetData(Range), GetNum(Range), Value, IdentityFunctor(), Less<>());
	}

	//////////////////////////////////////////////////////////////////////////
	// Upper Bound with self-define projection type

	template <typename RangeType, typename ValueType, typename ProjectionType, typename SortPredicateType>
	FORCEINLINE UINT32 UpperBoundBy(RangeType& Range, ValueType const& Value, ProjectionType Projection, SortPredicateType SortPredicate)
	{
		return AlgoImpl::UpperBoundInternal(GetData(Range), GetNum(Range), Value, Projection, SortPredicate);
	}
	template <typename RangeType, typename ValueType, typename ProjectionType>
	FORCEINLINE UINT32 UpperBoundBy(RangeType& Range, ValueType const& Value, ProjectionType Projection)
	{
		return AlgoImpl::UpperBoundInternal(GetData(Range), GetNum(Range), Value, Projection, Less<>());
	}

	//////////////////////////////////////////////////////////////////////////
	// Binary Search with with default projection type

	template <typename RangeType, typename ValueType, typename SortPredicateType>
	FORCEINLINE UINT32 BinarySearch(RangeType& Range, ValueType const& Value, SortPredicateType SortPredicate)
	{
		SIZE_T CheckIndex = LowerBound(Range, Value, SortPredicate);
		if (CheckIndex < GetNum(Range))
		{
			auto&& CheckValue = GetData(Range)[CheckIndex];
			// Since we returned lower bound we already know Value <= CheckValue. So if Value is not < CheckValue, they must be equal
			if (!SortPredicate(Value, CheckValue))
			{
				return CheckIndex;
			}
		}
		return INDEX_NONE;
	}
	template <typename RangeType, typename ValueType>
	FORCEINLINE UINT32 BinarySearch(RangeType& Range, ValueType const& Value)
	{
		return BinarySearch(Range, Value, TLess<>());
	}

	//////////////////////////////////////////////////////////////////////////
	// Binary Search with with self-define projection type

	template <typename RangeType, typename ValueType, typename ProjectionType, typename SortPredicateType>
	FORCEINLINE UINT32 BinarySearch(RangeType& Range, ValueType const& Value, ProjectionType Projection, SortPredicateType SortPredicate)
	{
		SIZE_T CheckIndex = LowerBound(Range, Value, Projection, SortPredicate);
		if (CheckIndex < GetNum(Range))
		{
			auto&& CheckValue = Invoke(Projection, GetData(Range)[CheckIndex]);
			// Since we returned lower bound we already know Value <= CheckValue. So if Value is not < CheckValue, they must be equal
			if (!SortPredicate(Value, CheckValue))
			{
				return CheckIndex;
			}
		}
		return INDEX_NONE;e
	}
	template <typename RangeType, typename ProjectionType, typename ValueType>
	FORCEINLINE UINT32 BinarySearch(RangeType& Range, ValueType const& Value, ProjectionType Projection)
	{
		return BinarySearch(Range, Value, Projection, Less<>());
	}
}