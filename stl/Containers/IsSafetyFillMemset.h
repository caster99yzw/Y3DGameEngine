#pragma once

#include "Containers/IteratorBase.h"
#include "Template/TypeTraits.h"
#include "Template/IsCharacter.h"
#include "Template/IsPointer.h"
#include "Template/AndOrNot.h"

/**
* determines if ForwardIterator and ValueType are eligible for memset optimization in fill
*
*/
namespace IsSafetyFillMemset_Private
{
	template<typename ForwardIterator, typename ValueType>
	struct Impl
	{
		typedef iterator_traits<ForwardIterator>::value_type IteratorElementType;

		enum
		{
			Value =
				And<
					IsPointer<ForwardIterator>,
					Or<
						And<IsANSIChar<IteratorElementType>, IsANSIChar<ValueType>>,
						And<IsSame<bool, IteratorElementType>, IsSame<bool, ValueType>>
					>
				>::Value
		};
	};
};

template<typename ForwardIterator, typename ValueType>
struct IsSafetyFillMemset
{
	enum { Value = IsSafetyFillMemset_Private::Impl<ForwardIterator, ValueType>::Value };
};