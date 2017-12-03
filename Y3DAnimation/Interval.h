#pragma once
#include "CoreTypes.h"
#include "BinarySearch.h"

struct IntervalType
{
	enum
	{
		LEFT_OPEN			= 0,
		RIGHT_OPEN			= 1,
		LEFT_CLOSE			= 2,
		RIGHT_CLOSE			= 3,
		BOTH_OPEN			= LEFT_OPEN | RIGHT_OPEN,
		BOTH_CLOSE			= LEFT_CLOSE | RIGHT_CLOSE,
		LEFT_HALF_OPEN		= LEFT_OPEN | RIGHT_CLOSE,
		RIGHT_HALF_OPEN		= RIGHT_OPEN | LEFT_CLOSE
	};
};

template <typename IndexType, typename RangeType, typename ValueType>
FORCEINLINE bool GetInterval(RangeType& Range, ValueType const& Value, IntervalType type, IndexType& Begin, IndexType& End) 
{
	//	the range is sorted from largest to smallest in default
	Begin = (type & IntervalType::LEFT_CLOSE)
		? Algo::LowerBound(Range, Value)
		: Algo::UpperBound(Range, Value);

	End = (type & IntervalType::RIGHT_OPEN)
		? Algo::LowerBound(Range, Value)
		: Algo::UpperBound(Range, Value);

	return (Begin + End) > 0;
}

template <typename IndexType, typename RangeType, typename ValueType>
FORCEINLINE bool GetInterval(RangeType& Range, ValueType const& LeftValue, ValueType const& RightValue, IntervalType type, IndexType& Begin, IndexType& End)
{
	//	the range is sorted from largest to smallest in default
	Begin = (type & IntervalType::LEFT_CLOSE)
		? Algo::LowerBound(Range, LeftValue)
		: Algo::UpperBound(Range, LeftValue);

	End = (type & IntervalType::RIGHT_OPEN)
		? Algo::LowerBound(Range, RightValue)
		: Algo::UpperBound(Range, RightValue);

	return (Begin + End) > 0;
}

template <typename IndexType, typename RangeType, typename ValueType, typename SortPredicateType>
FORCEINLINE bool GetInterval(RangeType& Range, const ValueType& Value, SortPredicateType SortPredicate, IntervalType type, IndexType& begin, IndexType& end) 
{
	// this is needed to test...
	// ...
	begin = (type & IntervalType::LEFT_CLOSE)
		? Algo::LowerBound(Range, Value, SortPredicate)
		: Algo::UpperBound(Range, Value, SortPredicate);

	end = (type & IntervalType::RIGHT_OPEN)
		? Algo::LowerBound(Range, Value, SortPredicate)
		: Algo::UpperBound(Range, Value, SortPredicate);

	return (begin + end) > 0;
}