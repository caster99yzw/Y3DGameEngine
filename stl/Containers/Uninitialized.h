#pragma once
#include "stl/CoreTypes.h"
#include "stl/Containers/IteratorBase.h"
#include "stl/Containers/Construct.h"
#include "stl/Template/IsPODType.h"
#include <vcruntime_string.h>


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
#include "Allocator.h"
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
	 return UninitializedCopy_P	rivate::UninitializedCopyImpl(First, Last, Result, value_type(Result));
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

	
		// FUNCTION TEMPLATE _Uninitialized_fill_n WITH ALLOCATOR
template<class FowardIterator,
	class DifferenceType,
	class AllocType> inline
	FowardIterator _Uninit_alloc_fill_n1(const FowardIterator _First, DifferenceType _Count, const _Iter_value_t<FowardIterator>& _Val,
		AllocType& _Al, std::false_type)
	{	// copy _Count copies of _Val to raw _First, using _Al, no special optimization
	UninitializedHepler<FowardIterator, AllocType> _Backout{_First, _Al};
	for (; 0 < _Count; --_Count)
		{
		_Backout._Emplace_back(_Val);
		}

	return (_Backout._Release());
	}

template<class FowardIterator,
	class DifferenceType,
	class AllocType> inline
	FowardIterator _Uninit_alloc_fill_n1(const FowardIterator _First, const DifferenceType _Count, const _Iter_value_t<FowardIterator>& _Val,
		AllocType&, std::true_type)
	{	// copy _Count copies of _Val to raw _First, using default _Alloc construct, memset optimization
	memset(_First, static_cast<unsigned char>(_Val), _Count);
	return (_First + _Count);
	}

template<class FowardIterator,
	class DifferenceType,
	class AllocType> inline
	FowardIterator _Uninitialized_fill_n(const FowardIterator _First, const DifferenceType _Count, const _Iter_value_t<FowardIterator>& _Val,
		AllocType& _Al)
	{	// copy _Count copies of _Val to raw _First, using _Al
	return (_Uninit_alloc_fill_n1(_First, _Count, _Val,	 _Al,
		std::_Conjunction_t<decltype(std:; _Fill_memset_is_safe(_First, _Val)),
			std::_Uses_default_construct<AllocType, decltype((_First)), decltype(_Val)>>()));
	}



namespace y3dcommon
{
	//////////////////////////////////////////////////////////////////////////

	template<class FowardIterator>
	using UseMemsetValue_t = std::_Conjunction_t<
		std::is_pointer<FowardIterator>,
		std::is_scalar<_Iter_value_t<FowardIterator>>,
		std::negation<std::is_volatile<_Iter_value_t<FowardIterator>>>,
		std::negation<std::is_member_pointer<_Iter_value_t<FowardIterator>>>>;

	//////////////////////////////////////////////////////////////////////////

	template<class FowardIterator>
	class UninitializedHepler
	{	
	public:
		UninitializedHepler(FowardIterator inFirst)
			: first(inFirst)
			, last(inFirst)
		{
		}

		UninitializedHepler(const UninitializedHepler&) = delete;
		UninitializedHepler& operator=(const UninitializedHepler&) = delete;

		~UninitializedHepler()
		{	
			DestroyRange(first, last);
		}

		template<class... U>
		void Emplace(U &&... inValues)
		{	
			Construct(last, std::forward<U1>(inValues)...);
			++last;
		}

		FowardIterator Release()
		{	
			first = last;
			return (last);
		}

	private:
		FowardIterator first;
		FowardIterator last;
	};


	template<class FowardIterator>
	FORCEINLINE FowardIterator AssignRangeWithZero(FowardIterator const firstIter, FowardIterator const lastIter)
	{
	std::vector<int> sfs;
		char * const first = reinterpret_cast<char *>(firstIter);
		char * const last = reinterpret_cast<char *>(lastIter);
		memset(first, 0, last - first);
		return (lastIter);
	}

	template<class FowardIterator, class DifferenceType>
	FORCEINLINE FowardIterator UninitializedConstructImpl(FowardIterator const first, DifferenceType count, std::false_type)
	{
		UninitializedHepler<FowardIterator> helper(first);
		for (; 0 < count; --count)
		{
			helper.Emplace();
		}
		return (helper.Release());
	}

	template<class FowardIterator, class DifferenceType>
	FORCEINLINE FowardIterator UninitializedConstructImpl(FowardIterator first, DifferenceType count, std::true_type)
	{
		return (AssignRangeWithZero(first, first + count));
	}

	template<class FowardIterator, class DifferenceType>
	FORCEINLINE FowardIterator UninitializedConstruct(FowardIterator first, DifferenceType count)
	{	
		return (UninitializedConstructImpl(first, count, UseMemsetValue_t<FowardIterator>()));
	};
}



//////////////////////////////////////////////////////////////////////////