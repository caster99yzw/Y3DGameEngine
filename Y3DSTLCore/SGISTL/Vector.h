#pragma once

#include "SGISTL/Allocator.h"
#include "SGISTL/Construct.h"
#include "SGISTL/Uninitialized.h"
#include "Template/IsArithmetic.h"

// why????????
#include "../../Y3DCommon/Y3DMathFunc.h"

template <typename T, typename Alloc>
class vector_base
{
public:
	typedef Alloc					allocator_type;
	static allocator_type get_allocator() { return allocator_type(); }

	vector_base(allocator_type const&)
		: M_start(0), M_finish(0), M_end_of_storage(0) {}
	vector_base(size_t n, allocator_type const&)
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
	void M_deallocate(T* p, size_t n) 
		{ return M_data_allocator::deallocate(p, n);	}


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
	static allocator_type get_allocator() { return base::get_allocator(); }

protected:

	void M_insert_aux(iterator position, value_type const& x);
	void M_insert_aux(iterator position);
	void M_fill_insert(iterator pos, size_type n, T const& x);

	
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
		M_finish = UninitializedCopy(x.begin(), x.end(), M_start);
	}

	vector(T const* first, T const* last, const allocator_type& a = allocator_type())
		: base(last - first, a)
	{
		M_finish = UninitializedCopy(first, last, M_start);
	}

	~vector() { Destroy(M_start, M_finish); }

	vector<T, Alloc>& operator = (vector<T, Alloc> const& x);
	void reserve(size_type n)
	{
		size_type old_size = size();
		iterator result = M_allocate(n);
		UninitializedCopy(M_start, M_finish, result);
		Destroy(M_start, M_finish);
		M_deallocate(M_start, M_end_of_storage - M_start);
		M_start = result;
		M_finish = result + old_size;
		M_end_of_storage = M_start + n;
	}

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

	iterator insert(iterator position, T const& x)
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

	void insert(iterator pos, size_type n, const T& x)
	{
		M_fill_insert(pos, n, x);
	}
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
};

template <typename T, typename Alloc>
FORCEINLINE bool operator == (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs)
{
	return lhs.size() == y.size();
}

template <typename T, typename Alloc>
FORCEINLINE bool operator < (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs)
{
	return lhs.size() < y.size();
}

template <typename T, typename Alloc>
FORCEINLINE bool operator > (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs)
{
	return rhs < lhs;
}

template <typename T, typename Alloc>
FORCEINLINE bool operator != (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) 
{
	return !(lhs == rhs);
}

template <typename T, typename Alloc>
FORCEINLINE bool operator <= (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) 
{
	return !(rhs < lhs);
}

template <typename T, typename Alloc>
FORCEINLINE bool operator >= (vector<T, Alloc> const& lhs, vector<T, Alloc> const& rhs) 
{
	return !(lhs < rhs);
}

template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (vector<T, Alloc> const& x)
{
	if (&x != this)
	{
		size_type const xlen = x.size();
		if (xlen > capacity())
		{
			iterator result = M_allocate(xlen);
			UninitializedCopy(x.begin(), x.end(), result);
			Destroy(M_start, M_finish);
			M_deallocate(M_start, M_end_of_storage - M_start);
			M_start = result;
			M_end_of_storage = M_start + xlen;
		}
		else if (size() >= xlen)
		{
			iterator pos = UninitializedCopy(x.begin(), x.end(), begin());
			Destroy(pos, M_finish);
		}
		else
		{
			UninitializedCopy(x.begin(), x.begin() + size(), begin());
			UninitializedCopy(x.begin() + size(), x.end(), end());
		}
		M_finish = M_start + xlen;
	}
	return *this;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::M_insert_aux(iterator position, value_type const& x) 
{
	if (M_finish != M_end_of_storage)
	{
		Construct(M_finish, *(M_finish - 1));
		++M_finish;	
		T xCopy = x;
		UninitializedCopy(position, M_finish - 1, position + 1);
		*position = xCopy;
	}
	else
	{
		size_type const old_size = size();
		size_type const len = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = M_allocate(len);
		iterator new_finish = new_start;
		
		new_finish = UninitializedCopy(M_start, position, new_start);
		Construct(new_finish, x);
		++new_finish;
		new_finish = UninitializedCopy(position, M_finish, new_finish);

		M_deallocate(M_start, M_end_of_storage - M_start);
		M_start = new_start;
		M_finish = new_finish;
		M_end_of_storage = new_start + len;
	}
}

template <typename T, typename Alloc>
void vector<T, Alloc>::M_insert_aux(iterator position)
{
	if (M_finish != M_end_of_storage)
	{
		Construct(M_finish, *(M_finish - 1));
		++M_finish;
		UninitializedCopy(position, M_finish - 1, position + 1);
		*position = T();
	}
	else
	{
		size_type const old_size = size();
		size_type const len = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = M_allocate(len);
		iterator new_finish = new_start;

		new_finish = UninitializedCopy(M_start, position, new_start);
		Construct(new_finish);
		++new_finish;
		new_finish = UninitializedCopy(position, M_finish, new_finish);

		M_deallocate(M_start, M_end_of_storage - M_start);
		M_start = new_start;
		M_finish = new_finish;
		M_end_of_storage = new_start + len;
	}
}

template <typename T, typename Alloc>
void vector<T, Alloc>::M_fill_insert(iterator pos, size_type n, T const& x)
{
	if (n == 0) return;
	
	if (size_type(M_end_of_storage - M_finish) >= n)
	{
		T xCopy = x;
		size_type const elems_after = M_finish - pos;
		iterator old_finish = M_finish;
		if (elems_after > n)
		{
			UninitializedCopy(M_finish - n, M_finish, M_finish);
			M_finish += n;
			UninitializedCopy(pos, old_finish, pos + n);
			UninitializedFill(pos, pos + n, xCopy);
		}
		else
		{
			UninitializedFill_n(M_finish, n - elems_after, xCopy);
			M_finish += n - elems_after;
			UninitializedCopy(pos, old_finish, M_finish);
			M_finish += elems_after;
			UninitializedFill(pos, old_finish, xCopy);
		}
	}
	else
	{
		size_type const old_size = size();
		size_type const len = old_size + Y3D::Max(old_size, n);
		iterator new_start = M_allocate(len);
		iterator new_finish = new_start;

		new_finish = UninitializedCopy(M_start, pos, new_start);
		new_finish = UninitializedFill_n(new_finish, n, x);
		new_finish = UninitializedCopy(pos, M_finish, new_finish);

		M_deallocate(M_start, M_end_of_storage - M_start);
		M_start = new_start;
		M_finish = new_finish;
		M_end_of_storage = new_start + len;
	}
}