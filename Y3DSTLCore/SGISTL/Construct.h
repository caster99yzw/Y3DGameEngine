#pragma once
#include <new>
#include "CoreTypes.h"
#include "SGISTL/IteratorBase.h"
#include "Template/EnableIf.h"
#include "Template/IsTriviallyDestructible.h"

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
