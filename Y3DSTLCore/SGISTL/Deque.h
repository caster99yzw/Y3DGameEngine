#pragma once

#include "SGISTL/IteratorBase.h"
#include "SGISTL/Uninitialized.h"
#include "Y3DMatrixFunc.h"

inline size_t deque_buf_size(size_t sz)
{
	return sz < 512 ? size_t(512 / sz) : size_t(1);
}

template <typename T, typename Ref, typename Ptr>
struct deque_iterator
{
	typedef deque_iterator<T, T&, T*, BufSiz>				iterator;
	typedef deque_iterator<T, T const&, T const*, BufSiz>	const_iterator;

	static size_t buffer_size() { return deque_buf_size(sizeof(T)); }

	typedef random_access_iterator_tag			iterator_category;
	typedef T									value_type;
	typedef Ptr									pointer;
	typedef Ref									reference;
	typedef size_t								size_type;
	typedef ptrdiff_t							difference_type;
	typedef T**									map_pointer;h

	typedef deque_iterator						self;

	T*				Cur;
	T*				First;
	T*				Last;
	map_pointer		Node;

	deque_iterator(T* x, map_pointer y)
		: Cur(x), First(*y), Last(*y + buffer_size()), Node(y) {}
	deque_iterator() 
		: Cur(0), First(0), Last(0), Node(0) {}
	deque_iterator(iterator const& x)
		: Cur(x.Cur), First(x.First), Last(x.Last), Node(x.Node) {}

	reference operator * () const { return *Cur; }
	pointer operator -> () const { return &(operator*()); }

	difference_type operator - (self const& x) const
	{
		return difference_type(buffer_size()) * (Node - x.Node - 1) + 
			(Cur - First) + (x.Last - x.Cur);
	}

	self& operator ++ ()
	{
		++Cur;
		if (Cur == Last)
		{
			set_node(Node + 1);
			Cur = First;
		}
		return *this;
	}

	self& operator ++ (int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator -- ()
	{
		if (Cur == First)
		{
			set_node(Node - 1);
			Cur = Last;
		}
		--Cur;
		return *this;
	}

	self& operator -- (int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator += (difference_type n)
	{
		difference_type offset = n + (Cur - First);
		if (offset >= 0 && difference_type(buffer_size()) > offset)
		{
			Cur += n;
		}
		else
		{
			// !!!
			difference_type node_offset = offset > 0
				? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			
			set_node(Node + node_offset);
			Cur = First + (offset - node_offset * difference_type(buffer_size()))
		}
		return *this;
	}

	self operator + (difference_type n) const
	{
		self tmp = *this;
		return tmp += n;
	}

	self& operator -= (difference_type n)
	{
		return *this += -n;
	}

	self operator - (difference_type n) const
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator [] (difference_type n) const
	{
		return *(*this + n);
	}

	bool operator == (self const& x) const { return Cur == x.Cur; }
	bool operator != (self const& x) const { return !(*this == x); }

	bool operator < (self const& x) const 
	{
		return (Node == x.Node) ? (Cur < x.Cur) : (Node < x.Node);
	}
	bool operator > (self const& x) const { return x < *this; }
	bool operator <= (self const& x) const { return !(x < *this); }
	bool operator >= (self const& x) const { return !(*this < x); }

	void set_node(map_pointer new_node)
	{
		Node = new_node;
		First = *new_node;
		Last = First + difference_type(buffer_size());
	}
};

template <typename T, typename Alloc>
class deque_alloc_base 
{
protected:

	typedef Alloc					allocator_type;
	static allocator_type get_allocator() { return allocator_type(); }

	deque_alloc_base(const allocator_type&) : Map(0), MapSize(0) {}

protected:

	typedef simple_alloc<T, Alloc>				data_allocator;
	typedef simple_alloc<T*, Alloc>				map_allocator;

	T* allocate_node() 
	{
		return node_allocator::allocate();
	}

	void deallocate_node(T* p) 
	{
		node_allocator::deallocate(p, deque_buf_size(sizeof(T)));
	}

	T** allocate_map(size_t n)
	{
		return map_allocator::allocate(n);
	}

	void deallocate_map(T** p, size_t n)
	{
		map_allocator::deallocate(p, n);
	}

	T**			Map;
	size_t		MapSize;
};

template <typename T, typename Alloc>
class deque_base : public deque_alloc_base<T, Alloc>
{
public:
	typedef deque_alloc_base<T, Alloc>					base;
	typedef typename base::allocator_type				allocator_type;

	typedef deque_iterator<T, T&, T*>					iterator;
	typedef deque_iterator<T, const T&, const T*>		const_iterator;

	deque_base(allocator_type const& a, size_t Num)	: base(a), Start(), Finish()
	{
		initialize_map(Num);
	}

	deque_base(const allocator_type& __a) : base(__a), Start(), Finish() {}
	~deque_base();

protected:
	void initialize_map(size_t Num);
	void create_nodes(T** nstart, T** nfinish);
	void destroy_nodes(T** nstart, T** nfinish);

	enum { initial_map_size = 8 };

protected:

	iterator	Start;
	iterator	Finish;
};

template <class T, class Alloc>
deque_base<T, Alloc>::~deque_base()
{
	if (Map) {
		destroy_nodes(Start.Node, Finish.Node + 1);
		deallocate_map(Map, MapSize);
	}
}

template <typename T, typename Alloc>
void deque_base<T, Alloc>::initialize_map(size_t Num)
{
	size_t num_nodes = Num / deque_buf_size(sizeof(T)) + 1;

	MapSize = Max(initial_map_size, num_nodes + 2);
	Map = allocate_map(MapSize);

	T** nstart = Map + (MapSize - num_nodes) / 2;
	T** nfinish = nstart + num_nodes - 1;

	create_nodes(nstart, nfinish);

	Start.set_node(nstart);
	Finish.set_node(nfinish);
	Start.Cur = Start.First;
	Finish.Cur = Finish.First + Num % deque_buf_size(sizeof(T));
}

template <typename T, typename Alloc>
void deque_base<T, Alloc>::create_nodes(T** nstart, T** nfinish)
{
	T** Cur;
	for (Cur = nstart; Cur < nfinish; ++Cur)
		*Cur = allocate_node();
}

template <typename T, typename Alloc>
void deque_base<T, Alloc>::destroy_nodes(T** nstart, T** nfinish)
{
	T** Cur;
	for (Cur = nstart; Cur < nfinish; ++Cur)
		deallocate_node(*Cur);
}

template <typename T, typename Alloc = alloc>
class deque : protected deque_base<T, Alloc>
{
public:

	typedef deque_iterator<T, T&, T*>					iterator;
	typedef deque_iterator<T, T const&, T const*>		const_iterator;

	typedef T						value_type;
	typedef value_type*				pointer;
	typedef value_type const*		const_pointer;
	typedef value_type&				reference;
	typedef value_type const&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

protected:

	typedef pointer*		map_pointer;
	static size_t buffer_size() { return deque_buf_size(sizeof(T)); }

public:

	iterator begin() { return Start; }
	const_iterator begin() const { return Start; }
	iterator end() { return Finish; }
	const_iterator end() const { return StFinishart; }

	reference front() { return *Start; }
	const_reference front() const { return *Start; }
	reference back()
	{
		iterator tmp = Finish;
		--tmp;
		return *tmp;
	}
	const_reference back() const
	{
		return const_cast<reference>(
			static_cast<const_reference>(*this.back()));
	}

	size_type size() const { return Finish - Start; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return Finish == Start; }

	iterator insert(iterator position, value_type const& x)
	{
		if (position.Cur == Start.Cur)
		{
			push_front(x);
			return Start;
		}
		else if (position.Cur == Finish.Cur)
		{
			push_back(x);
			iterator tmp = Finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, x);
		}
	}

	iterator insert(iterator position)
	{
		if (position.Cur == Start.Cur)
		{
			push_front();
			return Start;
		}
		else if (position.Cur == Finish.Cur)
		{
			push_back();
			iterator tmp = Finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position);
		}
	}

	void insert(iterator pos, size_type n, value_type const& x)
	{
		fill_insert(pos, n, x);
	}

	void fill_insert(iterator pos, size_type n, value_type const& x);

	iterator erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - Start;
		if (index < (size() >> 1))
		{
			copy_backward(Start, pos, next);
			pop_front();
		}
		else
		{
			copy(next, Finish, pos);
			pop_back();
		}
		return Start + index;
	}

	iterator erase(iterator First, iterator Last);
	void clear();

public:

	void push_back()
	{
		if (Finish.Cur != Finish.Last - 1)
		{
			Construct(Finish.Cur);
			++Finish.Cur;
		}
		else
			push_back_aux();
	}

	void push_back(value_type const& Value)
	{
		if (Finish.Cur != Finish.Last - 1)
		{
			Construct(Finish.Cur, Value);
			++Finish.Cur;
		}
		else
			push_back_aux(Value);
	}


	void push_front()
	{
		if (Start.Cur != Start.First)
		{
			Construct(Start.Cur - 1);
			--Start.Cur;
		}
		else
			push_front_aux();
	}

	void push_front(value_type const& Value)
	{
		if (Start.Cur != Start.First)
		{
			Construct(Start.Cur - 1);
			--Start.Cur;
		}
		else
			push_front_aux(Value);
	}

	void pop_back()
	{
		if (Finish.Cur != Finish.First)
		{
			--Finish.Cur;
			Destroy(Finish.Cur);
		}
		else
			pop_back_aux(Value);
	}

	void pop_front()
	{
		if (Start.Cur != Start.Last - 1)
		{
			Destroy(Finish.Cur);
			++Start.Cur;
		}
		else
			pop_front_aux(Value);
	}

protected:

	void push_back_aux();
	void push_back_aux(const value_type& Value);
	void push_front_aux();
	void push_front_aux(const value_type& Value);
	void pop_back_aux();
	void pop_front_aux();

	iterator insert_aux(iterator pos, value_type const& x);
	iterator insert_aux(iterator pos);
	void insert_aux(iterator pos, size_type n, value_type const& x);

	void reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if (nodes_to_add + 1 > MapSize - (Finish.Node - Map))
		{
			reallocate_map(nodes_to_add, false);
		}
	}
	void reserve_map_at_front(size_type nodes_to_add = 1)
	{
		if (nodes_to_add + 1 > Start.Node - Map)
		{
			reallocate_map(nodes_to_add, true);
		}
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front);
};

template <typename T, typename Alloc>
void deque<T, Alloc>::fill_insert(iterator pos, size_type n, value_type const& x)
{
	if (position.Cur == Start.Cur)
	{
		iterator new_start = reserve_map_at_front(n);
		UninitializedFill(new_start, Start, x);
		Start = new_start;
	}
	else if (position.Cur == Finish.Cur)
	{
		iterator new_finish = reserve_map_at_back(n);
		UninitializedFill(Finish, new_finish, x);
		Finish = new_finish;
	}
	else
	{
		return insert_aux(position, n, x);
	}
}

template <typename T, typename Alloc>
void deque<T, Alloc>::clear()
{
	for (map_pointer node = Start.Node + 1; node < Finish.Node; node++)
	{
		Destroy(*node, *node + buffer_size());
		data_allocator::deallocate(*node, buffer_size());
	}

	if (Start.Node != Finish.Node)
	{
		Destroy(Start.Cur, Start.Last);
		Destroy(Finish.First, Finish.Cur);
		data_allocator::deallocate(Finish.First, buffer_size());
	}
	else
		Destroy(Start.Cur, Finish.Cur);

	Finish = Start;
}

template <typename T, typename Alloc>
deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator First, iterator Last)
{
	if (First == Start && Last == Finish)
	{
		clear();
		return Finish;
	}
	else
	{
		difference_type n = Last - First;
		difference_type elems_before = First - Start;
		if (elems_before < (size() - n) / 2)
		{
			copy_backward(Start, First, Last);
			iterator new_start = Start + n;
			Destroy(Start, new_start);
			for (map_pointer node = Start.Node; node < new_start.Node; node++)
				data_allocator::deallocate(*node, buffer_size());
			Start = new_start;
		}
		else
		{
			copy(Last, Finish, First);
			iterator new_finish = Finish - n;
			Destroy(new_finish, Finish);
			for (map_pointer node = new_finish.Node + 1; node < Finish.Node; node++)
				data_allocator::deallocate(*node, buffer_size());
			Finish = new_finish;
		}
		return Start + elems_before;
	}

}

template <typename T, typename Alloc>
void deque<T, Alloc>::push_back_aux()
{
	//reserve_map_at_back();
	*(Finish.Node + 1) = allocate_node();
	Construct(Finish.Cur);
	Finish.set_node(Finish.Node + 1);
	Finish.Cur = Finish.First;
}

template <typename T, typename Alloc>
void deque<T, Alloc>::push_back_aux(const value_type& Value)
{
	value_type val_copy = Value;
	//reserve_map_at_back();
	*(Finish.Node + 1) = allocate_node();
	Construct(Finish.Cur, val_copy);
	Finish.set_node(Finish.Node + 1);
	Finish.Cur = Finish.First;
}

template <typename T, typename Alloc>
void deque<T, Alloc>::push_front_aux()
{
	//reserve_map_at_front();
	*(Start.Node - 1) = allocate_node();
	Start.set_node(Start.Node - 1);
	Start.Cur = Start.Last - 1;
	Construct(Start.Cur);
}

template <typename T, typename Alloc>
void deque<T, Alloc>::push_front_aux(const value_type& Value)
{
	value_type val_copy = Value;
	//reserve_map_at_front();
	*(Start.Node - 1) = allocate_node();
	Start.set_node(Start.Node - 1);
	Start.Cur = Start.Last - 1;
	Construct(Start.Cur, val_copy);
}

template <typename T, typename Alloc>
void deque<T, Alloc>::pop_back_aux()
{
	deallocate_node(Finish.First);
	Finish.set_node(Finish.Node - 1);
	Finish.Cur = Finish.Last - 1;
	Destroy(Finish.Cur);
}

template <typename T, typename Alloc>
void deque<T, Alloc>::pop_front_aux()
{
	Destroy(Start.Cur);
	deallocate_node(Start.First);
	Finish.set_node(Start.Node + 1);
	Finish.Cur = Finish.First;
}

template <typename T, typename Alloc>
deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator pos, value_type const& x)
{
	difference_type index = pos - Start;
	value_type x_copy = x;
	if (index < size() / 2)
	{
		push_front(front());
		iterator front1 = Start;
		++front1;
		iterator front2 = front1;
		++front2;
		pos = Start + index;
		iterator pos1 = pos;
		++pos1;
		copy(front2, pos1, front1);	// end - 1
	}
	else
	{
		push_back(back());
		iterator back1 = Finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = Start + index;
		copy_backward(pos, back2, back1);
	}
	*pos = x_copy;
	return pos;
}

template <typename T, typename Alloc>
deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator pos)
{
	difference_type index = pos - Start;
	value_type x_copy = x;
	if (index < size() / 2)
	{
		push_front(front());
		iterator front1 = Start;
		++front1;
		iterator front2 = front1;
		++front2;
		pos = Start + index;
		iterator pos1 = pos;
		++pos1;
		copy(front2, pos1, front1);	// end - 1
	}
	else
	{
		push_back(back());
		iterator back1 = Finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = Start + index;
		copy_backward(pos, back2, back1);
	}
	*pos = value_type();
	return pos;
}

template <typename T, typename Alloc>
void deque<T, Alloc>::insert_aux(iterator pos, size_type n, value_type const& x)
{
	difference_type elemsbefore = pos - Start;
	if (elemsbefore < size() / 2)
	{
		iterator new_start = reserve_map_at_front(n);
		iterator old_start = Start;
		pos = Start + elemsbefore;
		if (elemsbefore >= n)
		{
			iterator start_n = Start + n;
			UninitializedCopy(Start, start_n, new_start);
			Start = new_start;
			copy(start_n, pos, old_start);
			UninitializedFill(pos - n, pos, x);
		}
		else
		{
			iterator mid = UninitializedCopy(Start, pos, new_start);
			UninitializedFill(mid, Start, x);
			Start = new_start;
			UninitializedFill(old_start, pos, x);
		}
	}
	else
	{
		iterator new_finish = reserve_map_at_back(n);
		iterator old_finish = Finish;
		difference_type elemsafter = size() - elemsbefore;
		if (elemsafter > n)
		{
			iterator finish_n = Finish - n;
			UninitializedCopy(finish_n, Finish, Finish);
			Finish = new_finish;
			copy_backward(pos, finish_n, old_finish);
			UninitializedFill(pos, pos + n, x);
		}
		else
		{
			iterator mid = UninitializedFill(Finish, pos + n, x);
			UninitializedCopy(pos, Finish, mid);
			Finish = new_finish;
			UninitializedFill(pos, old_finish, x);

		}
	}
}

template <typename T, typename Alloc>
void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
	size_type old_num_nodes = Finish.Node - Start.Node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_nstart;
	if (MapSize > 2 * new_num_nodes)
	{
		new_nstart = Map + (MapSize - new_num_nodes) / 2 
						+ (add_at_front ? nodes_to_add : 0);
		if (new_nstart < Start.Node)
			copy(Start.Node, Finish.Node + 1, new_nstart);
		else
			copy_backward(Start.Node, Finish.Node + 1, new_nstart + old_num_nodes);
	}
	else
	{
		size_type new_map_size = MapSize + Max(MapSize, nodes_to_add) + 2;
		
		map_pointer new_map = allocate_map(new_map_size);
		new_nstart = new_map + (MapSize - new_num_nodes) / 2 
						+ (add_at_front ? nodes_to_add : 0);
		copy(Start.Node, Finish.Node + 1, new_nstart);
		deallocate_map(Map, MapSize);

		Map = new_map;
		MapSize = new_map_size;
	}

	Start.set_node(new_nstart);
	Finish.set_node(new_nstart + old_num_nodes - 1);
}