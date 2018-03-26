#pragma once

#include <vcruntime_string.h>
#include "CoreTypes.h"
#include "SGISTL/IteratorBase.h"
#include "SGISTL/Construct.h"
#include "Template/IsPODType.h"

namespace UninitializedCopy_Private
{
	template <typename InputIterator, typename ForwardIterator, typename SourceElementType>
	FORCEINLINE auto UninitializedCopyImpl(InputIterator First, InputIterator Last, ForwardIterator Result, SourceElementType* Point)
		-> typename EnableIf<IsPODType<SourceElementType>::Value, ForwardIterator>::Type
	{
		memmove(Result, First, sizeof(SourceElementType) * (Last - First));
		return Result + (Last - First);
	}

	template <typename InputIterator, typename ForwardIterator, typename SourceElementType>
	FORCEINLINE auto UninitializedCopyImpl(InputIterator First, InputIterator Last, ForwardIterator Result, SourceElementType* Point)
		-> typename EnableIf<!IsPODType<SourceElementType>::Value, ForwardIterator>::Type
	{
		ForwardIterator Cur = Result;
		for (; First != Last; ++First, ++Cur)
			Construct(&*Cur, *First);
		return Cur; 
	}

	template <typename InputIterator, typename SizeType, typename ForwardIterator>
	FORCEINLINE ForwardIterator UninitializedCopyImpl_n(InputIterator First, SizeType Count, ForwardIterator Result, input_iterator_tag)
	{
		ForwardIterator Cur = Result;
		for (; Count > 0; --Count, ++First, ++Cur)
			Construct(&*Cur, *First);
		return Cur;
	}

	template <typename RandomAccessIterator, typename SizeType, typename ForwardIterator>
	FORCEINLINE ForwardIterator UninitializedCopyImpl_n(RandomAccessIterator First, SizeType Count, ForwardIterator Result, random_access_iterator_tag)
	{
		RandomAccessIterator Last = First + Count;
		return ::UninitializedCopy(First, Last, Result);
	}
}

template <typename InputIterator, typename ForwardIterator>
FORCEINLINE ForwardIterator UninitializedCopy(InputIterator First, InputIterator Last, ForwardIterator Result)
{
	 return UninitializedCopy_Private::UninitializedCopyImpl(First, Last, Result, value_type(Result));
}

template <typename InputIterator, typename SizeType, typename ForwardIterator>
FORCEINLINE ForwardIterator UninitializedCopy_n(InputIterator First, SizeType Count, ForwardIterator Result)
{
	UninitializedCopy_Private::UninitializedCopyImpl_n(First, Count, Result, iterator_category(First));
}

//////////////////////////////////////////////////////////////////////////

namespace UninitializedFill_Private
{
	template <typename ForwardIterator, typename ValueType>
	FORCEINLINE auto UninitializedFillImpl(ForwardIterator First, ForwardIterator Last, ValueType const& Value)
		-> typename EnableIf<IsPODType<ValueType>::Value>::Type
	{
		  for (; First != Last; ++First) 
			*First = Value;
	}

	template <typename ForwardIterator, typename ValueType>
	FORCEINLINE auto UninitializedFillImpl(ForwardIterator First, ForwardIterator Last, ValueType const& Value)
		-> typename EnableIf<!IsPODType<ValueType>::Value>::Type
	{
		ForwardIterator Cur = Value;
		for (; Cur != Last; ++Cur) 
			Construct(&*Cur, Value);
		return Cur;
	}

	template <typename ForwardIterator, typename SizeType, typename ValueType>
	FORCEINLINE ForwardIterator UninitializedFillImpl_n(ForwardIterator First, SizeType Count, ValueType const& Value, input_iterator_tag)
	{
		ForwardIterator Cur = First;
		for (; Count > 0; --Count, ++Cur)
		{
			Construct(&*Cur, Value);
		}
		return Cur;
	}

	template <typename RandomAccessIterator, typename SizeType, typename ValueType>
	FORCEINLINE RandomAccessIterator UninitializedFillImpl_n(RandomAccessIterator First, SizeType Count, ValueType const& Value, random_access_iterator_tag)
	{
		RandomAccessIterator Last = First + Count;
		::UninitializedFill(First, Last, Value);
		return Last;
	}
}

template <typename ForwardIterator, typename ValueType>
FORCEINLINE void UninitializedFill(ForwardIterator First, ForwardIterator Last, ValueType const& Value)
{
	UninitializedFill_Private::UninitializedFillImpl(First, Last, Value);
}

template <typename ForwardIterator, typename SizeType, typename ValueType>
FORCEINLINE ForwardIterator UninitializedFill_n(ForwardIterator First, SizeType Count, ValueType const& Value)
{
	return UninitializedFill_Private::UninitializedFillImpl_n(First, Count, Value, iterator_category(First));
}

//////////////////////////////////////////////////////////////////////////