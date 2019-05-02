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

template<typename ClassType, typename... Types>
void Construct(ClassType * const _Ptr, Types &&... _Args)
{	
	::new (const_cast<void *>(static_cast<const volatile void *>(_Ptr))) ClassType(std::forward<Types>(_Args)...);
}

template <typename ValueType>
FORCEINLINE void Destroy(ValueType* Pointer)
{
	Destroy_Private::DestroyImpl(Pointer);
}


template <typename ForwardIterator>
FORCEINLINE void DestroyRangeImpl(ForwardIterator firstIter, ForwardIterator lastIter, std::false_type)
{	
	for (; firstIter != lastIter; ++firstIter)
	{
		Destroy_Private::DestroyImpl(firstIter, lastIter, value_type(firstIter));
	}
}

template <typename ForwardIterator>
FORCEINLINE void DestroyRangeImpl(ForwardIterator firstIter, ForwardIterator lastIter, std::true_type)
{	
	// nothing to do
}

template <typename ForwardIterator>
FORCEINLINE void DestroyRange(ForwardIterator firstIter, ForwardIterator lastIter)
{
	using ValueType = typename iterator_traits<ForwardIterator>::value_type*;
	DestroyRangeImpl(firstIter, lastIter, std::is_trivially_destructible<ValueType>());
}

FORCEINLINE void Destroy(char*, char*) {}
FORCEINLINE void Destroy(wchar_t*, wchar_t*) {}
FORCEINLINE void Destroy(int*, int*) {}
FORCEINLINE void Destroy(long*, long*) {}
FORCEINLINE void Destroy(float*, float*) {}
FORCEINLINE void Destroy(double*, double*) {}


template<typename AllocType>
	struct allocator_traits
		: std::conditional_t<std::_Is_default_allocator<AllocType>::value,
			std::_Default_allocator_traits<AllocType>, std::_Normal_allocator_traits<AllocType>>
	{	// defines traits for allocators
	};
