#pragma once
#include "stl/Containers/IteratorBase.h"
#include "stl/Template/EnableIf.h"
#include "stl/Template/IsTrivially.h"
#include "stl/CoreTypes.h"
#include <new>

namespace Destroy_Private
{
	//////////////////////////////////////////////////////////////////////////
	//	Destroy the pointer

	template <typename ValueType>
	FORCEINLINE typename EnableIf<!IsTriviallyDestructible<ValueType>::Value>::Type DestroyImpl(ValueType* Pointer)
	{
		// We need a typedef here because VC won't compile the destructor call below if ElementType itself has a member called ElementType
		typedef ValueType DestructItemsElementTypeTypedef;

		Pointer->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
	}

	template <typename ValueType>
	FORCEINLINE typename EnableIf<IsTriviallyDestructible<ValueType>::Value>::Type DestroyImpl(ValueType* Pointer)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	//	Destroy the container, such as vector, initiate list and array

	template <typename ForwardIterator, typename ValueType>
	FORCEINLINE typename EnableIf<!IsTriviallyDestructible<ValueType>::Value>::Type DestroyImpl(ForwardIterator FirstItor, ForwardIterator LastItor, ValueType*)
	{
		for (; FirstItor != LastItor; ++FirstItor)
			DestroyImpl(&*FirstItor);
	}

	template <typename ForwardIterator, typename ValueType>
	FORCEINLINE typename EnableIf<IsTriviallyDestructible<ValueType>::Value>::Type DestroyImpl(ForwardIterator FirstItor, ForwardIterator LastItor, ValueType*)
	{
	}
};

template <typename PointerType, typename ValueType>
FORCEINLINE void Construct(PointerType* Pointer, ValueType const& Value)
{
	new(Pointer) PointerType(Value);
}

template <typename ValueType>
FORCEINLINE void Destroy(ValueType* Pointer)
{
	Destroy_Private::DestroyImpl(Pointer);
}

template <typename ForwardIterator>
FORCEINLINE void Destroy(ForwardIterator FirstItor, ForwardIterator LastItor)
{
	Destroy_Private::DestroyImpl(FirstItor, LastItor, value_type(FirstItor));
}

FORCEINLINE void Destroy(char*, char*) {}
FORCEINLINE void Destroy(wchar_t*, wchar_t*) {}
FORCEINLINE void Destroy(int*, int*) {}
FORCEINLINE void Destroy(long*, long*) {}
FORCEINLINE void Destroy(float*, float*) {}
FORCEINLINE void Destroy(double*, double*) {}


template<class _Alloc>
	struct allocator_traits
		: std::conditional_t<std::_Is_default_allocator<_Alloc>::value,
			std::_Default_allocator_traits<_Alloc>, std::_Normal_allocator_traits<_Alloc>>
	{	// defines traits for allocators
	};

		// FUNCTION TEMPLATE _Destroy_range WITH ALLOC
template<class _Alloc> 
inline void _Destroy_range1(
		typename allocator_traits<_Alloc>::pointer _First,
		typename allocator_traits<_Alloc>::pointer _Last,
		_Alloc& _Al, std::false_type)
	{	// destroy [_First, _Last), no special optimization
	for (; _First != _Last; ++_First)
		{
		allocator_traits<_Alloc>::destroy(_Al, _Unfancy(_First));
		}
	}

template<class _Alloc> inline
	void _Destroy_range1(
		typename allocator_traits<_Alloc>::pointer,
		typename allocator_traits<_Alloc>::pointer,
		_Alloc&, std::true_type)
	{	// destroy [_First, _Last), trivially destructible and default destroy
		// nothing to do
	}

template<class _Alloc> inline
	void _Destroy_range(
		typename allocator_traits<_Alloc>::pointer _First,
		typename allocator_traits<_Alloc>::pointer _Last,
		_Alloc& _Al)
	{	// destroy [_First, _Last), choose optimization
		// note that this is an optimization for debug mode codegen;
		// in release mode the BE removes all of this
	using _Val = typename _Alloc::value_type;
	_Destroy_range1(_First, _Last, _Al, std::_Conjunction_t <
		std::is_trivially_destructible<_Val>,
		std::_Uses_default_destroy<_Alloc, _Val *>>());
	}
