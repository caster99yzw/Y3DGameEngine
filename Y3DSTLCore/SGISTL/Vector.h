#pragma once

#include "SGISTL/Allocator.h"
#include "SGISTL/Construct.h"
#include "SGISTL/Uninitialized.h"
#include "Template/IsArithmetic.h"

template <typename T, typename Alloc>
class vector_base
{
public:
	typedef Alloc					allocator_type;
	static allocator_type get_allocator() const { return allocator_type(); }

	vector_base(Alloc const&)
		: M_start(0), M_finish(0), M_end_of_storage(0) {}
	vector_base(size_t n, Alloc const&)
		: M_start(0), M_finish(0), M_end_of_storage(0) 
	{
		M_start = M_allocate(n);
		M_finish = M_start;
		M_end_of_storage = M_start + n;
	}

	~vector_base() { M_deallocate(M_start, M_end_of_storage - M_start); }

protected:
	T* M_start;
	T* M_finish;
	T* M_end_of_storage;

	typedef simple_alloc<T, Alloc>	M_data_allocator;
	T* M_allocate(size_t n)	
		{ return M_data_allocator::allocate(n); }
	T* M_deallocate(T* p, size_t n) 
		{ return M_data_allocator::deallcate(p, n);	}


};

template <typename T, typename Alloc = alloc>
class vector : protected vector_base<T, Alloc>
{
private:
	typedef vector_base<T, Alloc>	base;

public:
	typedef T						value_type;
	typedef value_type*				pointer;
	typedef value_type const*		const_pointer;
	typedef value_type*				iterator;
	typedef value_type const*		const_iterator;
	typedef value_type&				reference;
	typedef value_type const&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

	typedef typename base::allocator_type	allocator_type;
	allocator_type get_allocator() const { return base::get_allocator(); }

protected:

	void M_insert_aux(iterator position, value_type const& x) {}
	void M_insert_aux(iterator position) {}

public:

	iterator begin() { return M_start; }
	const_iterator begin() const { return M_start; }
	iterator end() { return M_finish; }
	const_iterator end() const { return M_finish; }

	size_type size() const { return size_type(end() - begin()); }
	size_type max_size() const { return size_type(-1) / sizeof(T); }
	size_type capacity() const { return size_type(M_end_of_storage - begin()); }

	bool empty() const { return begin() == end(); }

	reference operator[] (size_type n) { return *(begin() + n); }
	const_reference operator[] (size_type n) const { return *(begin() + n); }

	void M_range_check(size_type n) const 
	{
		if (n >= this->size())
			throw range_error("vector");
	}

	reference at(size_type n)
	{
		M_range_check(n); 
		return (*this)[n];
	}

	const_reference at(size_type n) const
	{
		M_range_check(n);
		return (*this)[n];
	}
	
	explicit vector(allocator_type const& a = get_allocator())
		: base(a) {}
	
	vector(size_type n, value_type const& value, allocator_type const& a = get_allocator()) 
		: base(n, a)
	{
		M_finish = UninitializedFill_n(M_start, n, value);
	}

	explicit vector(size_type n)
		: base(n, allocator_type())
	{
		M_finish = UninitializedFill_n(M_start, n, value);
	}

	vector(vector<T, Alloc> const& x)
		: base(x.size(), x.get_allocator())
	{
		M_finish = UninitializedCopy_n(x.begin(), n, M_start);
	}

	vector(T const* first, T const* last, const allocator_type& a = allocator_type())
		: base(last - first, a)
	{
		M_finish = uninitialized_copy(first, last, M_start);
	}

	~vector() { Destroy(M_start, M_finish); }

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	void push_back(value_type const& x)
	{
		if (M_finish != M_end_of_storage)
		{
			Construct(M_finish, x);
			++M_finish;
		}
		else
			M_insert_aux(end(), x);
	}

	void push_back()
	{
		if (M_finish != M_end_of_storage)
		{
			Construct(M_finish);
			++M_finish;
		}
		else
			M_insert_aux(end());
	}

	iterator insert(iterator position, T const* x)
	{
		size_type n = position - begin();
		if (M_finish != M_end_of_storage && position == end())
		{
			Construct(M_finish, x);
			++M_finish;
		}
		else
			M_insert_aux(position, x);
		return begin() + n;
	}

	iterator insert(iterator position)
	{
		size_type n = position - begin();
		if (M_finish != M_end_of_storage && position == end())
		{
			Construct(M_finish);
			++M_finish;
		}
		else
			M_insert_aux(position);
		return begin() + n;
	}

	//void insert(iterator position, const_iterator first, const_iterator __last);

	//void insert(iterator __pos, size_type __n, const _Tp& __x)
	//{
		//_M_fill_insert(__pos, __n, __x);
	//}

	//void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

	void pop_back()
	{
		--M_finish;
		Destroy(M_finish);
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
			copy(position + 1, M_finish, position);
		pop_back();
		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator pos = copy(last, M_finish, first);
		Destroy(pos, M_finish);
		M_finish = M_finish - (last - first);
		return first;
	}

	void resize(size_type new_size, value_type const& x)
	{
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}

	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }

protected:

	//
};