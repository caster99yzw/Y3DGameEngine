#pragma once

#include "SGISTL/Allocator.h"
#include "SGISTL/Construct.h"
#include "SGISTL/Uninitialized.h"

/*
* M_node of list is null node specifier, for begin is closed and end is open
*
*/


struct list_node_base
{
	list_node_base* M_prev;
	list_node_base* M_next;
};

template <typename T>
struct list_node : public list_node_base
{
	T M_data;
};


struct list_iterator_base
{
	typedef size_t							size_type;
	typedef ptrdiff_t						difference_type;
	typedef bidirectional_iterator_tag		iterator_category;

	list_node_base*							M_node;

	list_iterator_base(list_node_base* x) : M_node(x) {}
	list_iterator_base() {}

	void M_increment() { M_node = M_node->M_next; }
	void M_decrement() { M_node = M_node->M_prev; }

	bool operator == (list_iterator_base const& x) const
	{
		return M_node == x.M_node;
	}

	bool operator != (list_iterator_base const& x) const
	{
		return !(*this == x);
	}
};

template <typename T, typename Ref, typename Ptr>
struct list_iterator : public list_iterator_base
{
	typedef list_iterator<T, T&, T*>				iterator;
	typedef list_iterator<T, T const&, T const*>	const_iterator;
	typedef list_iterator<T, Ref, Ptr>				self;

	typedef	T										value_type;
	typedef Ptr										pointer;
	typedef Ref										reference;
	typedef list_node<T>*							link_type;

	list_iterator(link_type x) : list_iterator_base(x) {}
	list_iterator() {}
	list_iterator(iterator const& x) : list_iterator_base(x.M_node) {}

	reference operator * () const { return static_cast<link_type>(M_node)->M_data; }
	pointer operator -> () const { return &(operator*()); }

	self& operator ++ ()
	{
		this->M_increment();  
		return *this;
	}

	self operator ++ (int)
	{
		self tmp = *this;
		this->M_increment();
		return tmp;
	}

	self& operator -- ()
	{
		this->M_decrement();
		return *this;
	}

	self operator -- (int)
	{
		self tmp = *this;
		this->M_decrement();
		return *this;
	}
};

template <typename T, typename Alloc>
class list_base
{
public:
	typedef Alloc					allocator_type;
	static allocator_type get_allocator() { return allocator_type(); }

	list_base(allocator_type const&)
	{
		M_node = M_get_node();
		M_node->M_next = M_node;
		M_node->M_prev = M_node;
	}
	~list_base()
	{
		clear();
		M_put_node(M_node);
	}

	void clear();


protected:
	typedef simple_alloc<list_node<T>, Alloc>	M_data_allocator;
	list_node<T>* M_get_node() { return M_data_allocator::allocate(1); }
	void M_put_node(list_node<T>* p) { return M_data_allocator::deallocate(p, 1); }

protected:
	list_node<T>*					M_node;
};

template <typename T, typename Alloc>
void list_base<T, Alloc>::clear()
{
	list_node<T>* cur = static_cast<list_node<T>*>(M_node->M_next);
	while (cur != M_node)
	{
		list_node<T>* tmp = cur;
		cur = static_cast<list_node<T>*>(cur->M_next);
		Destroy(&tmp->M_data);
		M_put_node(tmp);
	}
	M_node->M_next = M_node;
	M_node->M_prev = M_node;
}

template <typename T, typename Alloc = alloc>
class list : protected list_base<T, Alloc>
{

	typedef list_base<T, Alloc>		base;

protected:

	typedef void*					void_pointer;

public:

	typedef list_node<T>			node_type;
	typedef list_node<T>*			node_pointer_type;

	typedef T						value_type;
	typedef value_type*				pointer;
	typedef value_type const*		const_pointer;
	typedef value_type&				reference;
	typedef value_type const&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

	typedef typename base::allocator_type	allocator_type;
	static allocator_type get_allocator() { return base::get_allocator(); }

public:

	typedef list_iterator<T, T&, T*>					iterator;
	typedef list_iterator<T, T const&, T const*>		const_iterator;


protected:

	node_pointer_type M_create_node(T const& x)
	{
		node_pointer_type p = M_get_node();
		Construct(&p->M_data, x);
		return p;
	}

	node_pointer_type M_create_node()
	{
		node_pointer_type p = M_get_node();
		Construct(&p->M_data);
		return p;
	}

	node_pointer_type M_destory_node(node_pointer_type p)
	{
		Destroy(&p->M_data);
		M_put_node(p);
	}

	void M_fill_insert(iterator pos, size_type n, T const& x);

public:

	iterator begin() { return static_cast<node_pointer_type>(M_node->M_next); }
	const_iterator begin() const { return static_cast<node_pointer_type>(M_node->M_next); }
	iterator end() { return M_node; }
	const_iterator end() const { return M_node; }

	bool empty() { return M_node->M_next == M_node; }
	size_type size() const
	{
		size_type result = 0;
		result = distance(begin(), end());
		return result;
	}
	size_type max_size() const { return size_type(-1) / sizeof(T); }

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(--end()); }
	const_reference back() const { return *(--end()); }

	void swap(list<T, Alloc>& x) { std::swap(M_node, x.M_node); }

	iterator insert(iterator pos, T const& x)
	{
		node_pointer_type tmp = M_create_node(x);
		tmp->M_next = pos.M_node;
		pos.M_node->M_prev->M_next = tmp;
		tmp->M_prev = pos.M_node->M_prev;
		pos.M_node->M_prev = tmp;
		return tmp;
	}

	iterator insert(iterator pos) { return insert(pos, T()); }

	void insert(iterator pos, size_type n, T const& x)
	{
		M_fill_insert(pos, n, x);
	}

	void insert(iterator pos, T const* first, T const* last);
	void insert(iterator pos, const_iterator first, const_iterator last);

	void push_front(T const& x) { insert(begin(), x); }
	void push_front() { insert(begin()); }
	void push_back(T const& x) { insert(end(), x); }
	void push_back() { insert(end()); }

	iterator erase(iterator pos)
	{
		list_node_base* next_node = pos.M_node->M_next;
		list_node_base* prev_node = pos.M_node->M_prev;
		node_pointer_type tmp = static_cast<node_pointer_type>(pos.M_node);
		prev_node->M_next = tmp->M_next;
		next_node->M_prev = tmp->M_prev;
		Destroy(&tmp->M_data);
		M_put_node(tmp);
		return iterator(static_cast<node_pointer_type>(next_node));
	}

	iterator erase(iterator first, iterator last);
	void clear() { base::clear(); }

	void pop_front() { erase(begin()); }
	void pop_back() 
	{
		iterator tmp = end(); 
		erase(--tmp);
	}


	explicit list(allocator_type const& a = get_allocator())
		: base(a) {}

	list(size_type n, T const& value, allocator_type const& a = get_allocator())
		: base(a)
	{
		insert(begin(), n, value);
	}

	explicit list(size_type n)
		: base(get_allocator())
	{
		insert(begin(), n, T());
	}

	list(T const* first, T const* last, const allocator_type& a = get_allocator())
		: base(last - first, a)
	{
		insert(begin(), first, last);
	}

	list(list<T, Alloc> const& x)
		: base(x.get_allocator())
	{
		insert(begin(), x.begin(), x.end());
	}

	list<T, Alloc>& operator = (list<T, Alloc> const& x)
	{
		insert(begin(), x.begin(), x.end());
	}

protected:
	void transfer(iterator pos, iterator first, iterator last)
	{
		if (pos != last)
		{
			// Remove [first, last) from its old position.
			last.M_node->M_prev->M_next = pos.M_node;
			first.M_node->M_prev->M_next = last.M_node;
			pos.M_node->M_prev->M_next = first.M_node;

			// Splice [first, last) into its new position.
			list_node_base* tmp = pos.M_node->M_prev;
			pos.M_node->M_prev = last.M_node->M_prev;
			last.M_node->M_prev = first.M_node->M_prev;
			first.M_node->M_prev = tmp;
		}
	}

public:	
	void splice(iterator pos, list& x)
	{
		if (!x.empty())
			transfer(pos, x.begin(), x.end());
	}

	void splice(iterator pos, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (pos == i || pos == j) return;
		transfer(pos, i, j);
	}

	void splice(iterator pos, list&, iterator first, iterator last)
	{
		if (first != last) 
			transfer(pos, first, last);
	}

	void remove(T const& value);
	void unique();
	void merge(list& x);
	void reverse();
	void sort();
};

template <typename T, typename Alloc>
FORCEINLINE bool operator == (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	typedef typename list<T, Alloc>::const_iterator const_iterator;
	const_iterator lhsItor = lhs.begin();
	const_iterator rhsItor = rhs.begin();
	while (lhsItor != lhs.end() && rhsItor != rhs.end() && *lhsItor == *rhsItor)
	{
		++lhsItor;
		++rhsItor;
	}
	return lhsItor == lhs.end() && rhsItor == rhs.end();
}

template <typename T, typename Alloc>
FORCEINLINE bool operator < (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	return lhs.size() < rhs.size();
}

template <typename T, typename Alloc>
FORCEINLINE bool operator > (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	return rhs < lhs;
}

template <typename T, typename Alloc>
FORCEINLINE bool operator != (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename Alloc>
FORCEINLINE bool operator <= (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	return !(rhs < lhs);
}

template <typename T, typename Alloc>
FORCEINLINE bool operator >= (list<T, Alloc> const& lhs, list<T, Alloc> const& rhs)
{
	return !(lhs < rhs);
}

template <typename T, typename Alloc>
void list<T, Alloc>::M_fill_insert(iterator pos, size_type n, T const& x)
{
	for (; n > 0; --n)
		insert(pos, x);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, T const* first, T const* last)
{
	for (; first != last; ++first)
		insert(pos, *first);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, const_iterator first, const_iterator last)
{
	for (; first != last; ++first)
		insert(pos, *first);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last)
{
	while (first != last)
		erase(first++);
	return last;
}

template <typename T, typename Alloc>
void list<T, Alloc>::remove(T const& value)
{
	iterator first = begin();
	iterator last = end();
	while (first != last)
	{
		iterator next = first;
		++next;
		if (*first == value) erase(first);
		first = next;
	}
}

template <typename T, typename Alloc>
void list<T, Alloc>::unique()
{
	iterator first = begin();
	iterator last = end();
	if (first == last) return;
	iterator next = first;
	while (++next != last)
	{
		if (*first == *next)
			erase(next);
		else
			first = next;
		next = first;
	}
}

template <typename T, typename Alloc>
void list<T, Alloc>::merge(list& x)
{
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	while (first1 != last1 && first2 != last2)
	{
		if (*first2 < *first1)
		{
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else
			++first1;
	}
	if (first2 != last2) transfer(last1, first2, last2);
}

template <typename T, typename Alloc>
void list<T, Alloc>::reverse()
{
	if (M_node->M_next == M_node || M_node->M_next->M_next == M_node)
		return;

	iterator first = begin();
	++first;
	while (first != end())
	{
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}

//merge sort
template <typename T, typename Alloc>
void list<T, Alloc>::sort()
{
	if (M_node->M_next == M_node || M_node->M_next->M_next = M_node)
		return;

	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;

	while (!empty())
	{
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty())
		{
			carry.merge(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill) ++fill;
	}

	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
	swap(counter[fill - 1]);
}
