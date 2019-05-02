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

		// STRUCT TEMPLATE _Uninitialized_backout_al
template<class _FwdIt,
	class _Alloc>
	class _Uninitialized_backout_al
	{	// struct to undo partially constructed ranges in _Uninitialized_xxx_al algorithms
public:
	_Uninitialized_backout_al(_FwdIt _Dest, _Alloc& _Al_)
		: _First(_Dest),
		_Last(_Dest),
		_Al(_Al_)
		{
		}

	_Uninitialized_backout_al(const _Uninitialized_backout_al&) = delete;
	_Uninitialized_backout_al& operator=(const _Uninitialized_backout_al&) = delete;

	~_Uninitialized_backout_al()
		{	// destroy all elements guarded by this instance
		_Destroy_range(_First, _Last, _Al);
		}

	template<class... _Types>
		void _Emplace_back(_Types&&... _Vals)
		{	// construct a new element at *_Last and increment
		allocator_traits<_Alloc>::construct(_Al, _Unfancy(_Last), _STD forward<_Types>(_Vals)...);
		++_Last;
		}

	_FwdIt _Release()
		{	// suppress any exception handling backout and return _Last
		_First = _Last;
		return (_Last);
		}

private:
	_FwdIt _First;
	_FwdIt _Last;
	_Alloc& _Al;
	};


		// FUNCTION TEMPLATE _Uninitialized_fill_n WITH ALLOCATOR
template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninit_alloc_fill_n1(const _FwdIt _First, _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc& _Al, std::false_type)
	{	// copy _Count copies of _Val to raw _First, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{_First, _Al};
	for (; 0 < _Count; --_Count)
		{
		_Backout._Emplace_back(_Val);
		}

	return (_Backout._Release());
	}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninit_alloc_fill_n1(const _FwdIt _First, const _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc&, std::true_type)
	{	// copy _Count copies of _Val to raw _First, using default _Alloc construct, memset optimization
	memset(_First, static_cast<unsigned char>(_Val), _Count);
	return (_First + _Count);
	}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_fill_n(const _FwdIt _First, const _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc& _Al)
	{	// copy _Count copies of _Val to raw _First, using _Al
	return (_Uninit_alloc_fill_n1(_First, _Count, _Val, _Al,
		std::_Conjunction_t<decltype(std:; _Fill_memset_is_safe(_First, _Val)),
			std::_Uses_default_construct<_Alloc, decltype((_First)), decltype(_Val)>>()));
	}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n1(const _FwdIt _First, _Diff _Count, _Alloc& _Al, std::false_type)
	{	// value-initialize _Count objects to raw _First, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{_First, _Al};
	for (; 0 < _Count; --_Count)
		{
		_Backout._Emplace_back();
		}

	return (_Backout._Release());
	}


template<class _FwdIt> inline
	_FwdIt _Zero_range(const _FwdIt _First, const _FwdIt _Last)
	{	// fill [_First, _Last) with zeroes
	char * const _First_ch = reinterpret_cast<char *>(_First);
	char * const _Last_ch = reinterpret_cast<char *>(_Last);
	memset(_First_ch, 0, _Last_ch - _First_ch);
	return (_Last);
	}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n1(_FwdIt _First, _Diff _Count, _Alloc&, std::true_type)
	{	// value-initialize _Count objects to raw _First, using default _Alloc construct, all-bits-zero type
	return (_Zero_range(_First, _First + _Count));
	}

		// FUNCTION TEMPLATE _Uninitialized_value_construct_n WITH ALLOCATOR
template<class _FwdIt>
	using _Use_memset_value_construct_t = std::_Conjunction_t<
		std::is_pointer<_FwdIt>,
		std::is_scalar<_Iter_value_t<_FwdIt>>,
		std::negation<std::is_volatile<_Iter_value_t<_FwdIt>>>,
		std::negation<std::is_member_pointer<_Iter_value_t<_FwdIt>>>>;

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n(_FwdIt _First, _Diff _Count, _Alloc& _Al)
	{	// value-initialize _Count objects to raw _First, using _Al
	return (_Uninitialized_value_construct_n1(_First, _Count, _Al,
		std::_Conjunction_t<_Use_memset_value_construct_t<_FwdIt>,
			std::_Uses_default_construct<_Alloc, decltype(_First)>>()));
	}


//////////////////////////////////////////////////////////////////////////