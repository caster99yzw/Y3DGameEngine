#pragma once
#include <climits>
#include <iostream>

#include "CoreTypes.h"

namespace Simple_Alloc_Private
{
	template <typename T>
	FORCEINLINE T* _allocate(ptrdiff_t size, T*)
	{
		set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0)
		{
			cerr << "out of memory" << endl;
			exit(1);
		}
		return tmp;
	}

	template <typename T>
	FORCEINLINE void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	template <typename T1, typename T2>
	FORCEINLINE void _construct(T1* p, T2 const& value)
	{
		new(p) T1(value);
	}

	template <typename T>
	FORCEINLINE void _destroy(T* ptr)
	{
		ptr->~T();
	}
}

template <typename T>
class simple_alloc
{
public:
	typedef T			value_type;
	typedef T*			pointer;
	typedef T const*	const_pointer;
	typedef T&			reference;
	typedef T const&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	//template <typedef U>
	//struct rebind
	//{
	//	typedef simple_alloc<U> other;
	//};

	pointer allocate(size_type n, void const* hint = 0)
	{
		return _allocate((difference_type)n, (pointer)0);
	}

	void deallocate(pointer p, size_type n)
	{
		_deallocate(p);
	}

	void construct(pointer p, T const& value)
	{
		_construct(p, value);
	}

	void destory(pointer p)
	{
		_destroy(p);
	}

	pointer address(reference x)
	{
		return (pointer)&x;
	}

	const_pointer const_address(const_reference x)
	{
		return (const_pointer)&x;
	}

	size_type max_size() const
	{
		return size_type(UINT_MAX / sizeof(T));
	}
};