#pragma once

#include "SGISTL/IteratorBase.h"
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

template <class T, class Alloc>
class deque_alloc_base 
{
protected:

	typedef Alloc					allocator_type;
	static allocator_type get_allocator() { return allocator_type(); }

	deque_alloc_base(const allocator_type&) : Map(0), MapSize(0) {}

protected:

	typedef simple_alloc<T, Alloc>				node_allocator;
	typedef simple_alloc<T*, Alloc>				map_allocator;

	T* allocate_node() 
	{
		return node_allocator::allocate(buffer));
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

template <class T, class Alloc>
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

template <class T, class Alloc>
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

template <class T, class Alloc>
void deque_base<T, Alloc>::create_nodes(T** nstart, T** nfinish)
{
	T** Cur;
	for (Cur = nstart; Cur < nfinish; ++Cur)
		*Cur = allocate_node();
}

template <class T, class Alloc>
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

public:

	typedef T						value_type;
	typedef value_type*				pointer;
	typedef value_type const*		const_pointer;
	typedef value_type&				reference;
	typedef value_type const&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

protected:

	typedef pointer*		map_pointer;

protected:

	map_pointer			Map;
	size_type			MapSize;

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
};

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