#pragma once
#include <climits>
#include <iostream>

#include "CoreTypes.h"

#define __THROW_BAD_ALLOC		fprintf(stderr, "out of memory\n"); exit(1)
//#define __THROW_BAD_ALLOC		throw std::bad_alloc()

namespace Alloc_Private
{
	template <int inst>
	class __malloc_alloc_template
	{
	private:

		//	OOM : out of memory
		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);
		static void(*__malloc_alloc_oom_handler)();

	public:

		static void* allocate(size_t n)
		{
			void* result = malloc(n);
			if (0 == result) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void* p, size_t /* __n */)
		{
			free(p);
		}

		static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz)
		{
			void* result = realloc(p, new_sz);
			if (0 == result) result = oom_realloc(p, new_sz);
			return result;
		}

		static void(*__set_malloc_handler(void(*f)()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return(old);
		}
	};

	template <int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

	template <int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void (*my_malloc_handle)();
		void* result;

		for (;;) 
		{
			my_malloc_handle = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handle) { __THROW_BAD_ALLOC; }
			(*my_malloc_handle)();
			result = malloc(n);
			if (result) return(result);
		}
	}

	template <int inst>
	void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
	{
		void(*my_malloc_handle)();
		void* result;

		for (;;)
		{
			my_malloc_handle = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handle) { __THROW_BAD_ALLOC; }
			(*my_malloc_handle)();
			result = realloc(p, n);
			if (result) return(result);
		}
	}

}
namespace Alloc_Private_1
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
}

//////////////////////////////////////////////////////////////////////////
//

template <typename T, typename Alloc>
class simple_alloc
{
public:
	static T* allocate(size_t n)
		{ return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T)); }

	static T* allocate(void)
		{ return (T*)Alloc::allocate(sizeof(T)); }

	static void deallocate(T* p, size_t n)
		{ if (0 != n) Alloc::deallocate(p, n * sizeof(T)); }

	static void deallocate(T* p)
		{ Alloc::deallocate(sizeof(T)); }
};


typedef Alloc_Private::__malloc_alloc_template<0>	malloc_alloc;

typedef malloc_alloc								alloc;

//////////////////////////////////////////////////////////////////////////
//

template <typename __Tp>
class Allocate
{
	typedef alloc			_Alloc;

public:
	typedef __Tp			value_type;
	typedef __Tp*			pointer;
	typedef __Tp const*		const_pointer;
	typedef __Tp&			reference;
	typedef __Tp const&		const_reference;
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

	template <typename __Tp1> struct rebind
	{
		typedef Allocate<__Tp1> other;
	};

	Allocate() {}
	Allocate(Allocate const&) {}
	template <typename __Tp1> Allocate(Allocate<__Tp1> const&) {}
	~Allocate() {}

	pointer address(reference __x) const { return &__x; }
	const_pointer address(const_reference __x) const { return &__x; }

	pointer allocate(size_type __n)
	{
		return (__n != 0)
			? static_cast<pointer>(_Alloc::allocate(__n * sizeof(value_type)))
			: 0;
	}

	void deallocate(pointer __p, size_type __n)
	{
		_Alloc::deallocate(__p, __n * sizeof(va));
	}

	size_type max_size() const
	{
		return size_type(UINT_MAX / sizeof(value_type));
	}

	void construct(pointer __p, value_type const& __val) { new(__p) value_type(__val); }
	void destory(pointer __p) { __p->~value_type(); }
};

template <>
class Allocate<void>
{
public:
	typedef void			value_type;
	typedef void*			pointer;
	typedef void const*		const_pointer;
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

	template <typename __Tp1> struct rebind
	{
		typedef Allocate<__Tp1> other;
	};
};