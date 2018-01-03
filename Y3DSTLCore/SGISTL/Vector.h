#pragma once

#include "SGISTL/Allocator.h"
#include "SGISTL/Construct.h"
#include "SGISTL/Uninitialized.h"

template <typename T, typename Alloc = alloc>
class vector
{
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

protected:

	typedef simple_alloc<value_type, Alloc> data_allocator;

	iterator						start;
	iterator						finish;
	iterator						end_of_storage;

	void insert_aux(iterator position, value_type const& x) {}

	void deallocate()
	{
		if (start) 
			data_allocator::deallocate(start, end_of_storage - start);
	}

	void fill_initialize(size_type n, value_type const& value)
	{
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

public:

	iterator begin() { return start; }
	const_iterator begin() const { return start; }
	iterator end() { return finish; }
	const_iterator end() const { return finish; }
	
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_storage - begin()); }

	bool empty() const { return begin() == end(); }

	reference operator[] (size_type n) { return *(begin() + n); }

	vector() : start(0), finish(0), end_of_storage(0) {}
	vector(size_type n, value_type const& value) { fill_initialize(n, value); }
	vector(int n, value_type const& value) { fill_initialize(n, value); }
	vector(long n, value_type const& value) { fill_initialize(n, value); }
	explicit vector(size_type n) { fill_initialize(n, value_type()); }

	~vector() 
	{
		Destroy(start, finish);
		deallocate();
	}

	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }

	void push_back(value_type const& x)
	{
		if (finish != end_of_storage)
		{
			Construct(finish, x);
			++finish;
		}
		else
			insert_aux(end(), x);
	}

	void pop_back()
	{
		--finish;
		Destroy(finish);
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
			copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
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

	iterator allocate_and_fill(size_type n, value_type const& x)
	{
		iterator result = data_allocator::allocate(n);
		UninitializedFill_n(result, n, x);
		return result;
	}
};