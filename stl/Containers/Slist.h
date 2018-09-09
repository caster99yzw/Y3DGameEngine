#pragma once

#include "Containers/IteratorBase.h"
#include "Template/Template.h"
#include "CoreTypes.h"

struct slist_node_base
{
	slist_node_base* next;
};

template <typename T>
struct slist_node : public slist_node_base
{
	T data;
};

FORCEINLINE slist_node_base* slist_make_link(
				slist_node_base* prev_node, 
				slist_node_base* new_node)
{
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

FORCEINLINE slist_node_base* slist_previous(
				slist_node_base* head,
				slist_node_base const* node)
{
	while (head && head->next != node)
		head = head->next;
	return head;
}

FORCEINLINE slist_node_base const* slist_previous(
				slist_node_base const* head,
				slist_node_base const* node)
{
	while (head && head->next != node)
		head = head->next;
	return head;
}

FORCEINLINE void slist_splice_after(
				slist_node_base* pos,
				slist_node_base* BeforeFirstNode,
				slist_node_base* BeforeLastNode)
{
	if (pos != BeforeFirstNode && pos != BeforeLastNode)
	{
		slist_node_base* First = BeforeFirstNode->next;
		slist_node_base* after = pos->next;
		BeforeFirstNode->next = BeforeLastNode->next;
		pos->next = First;
		BeforeLastNode->next = after;
	}
}

FORCEINLINE void slist_splice_after(
				slist_node_base* pos,
				slist_node_base* Head)
{
	slist_node_base* BeforeLastNode = slist_previous(Head, 0);
	if (BeforeLastNode != Head)
	{
		slist_node_base* after = pos->next;
		pos->next = Head->next;
		Head->next = 0;
		BeforeLastNode->next = after;
	}
}
FORCEINLINE slist_node_base* slist_reverse(slist_node_base* node)
{
	slist_node_base* result = node;
	node = node->next;
	result->next = 0;
	while (node)
	{
		slist_node_base* next = node->next;
		node->next = result;
		result = node;
		node = next;
	}
	return result;
}

FORCEINLINE size_t slist_size(slist_node_base* node)
{
	size_t result = 0;
	for (; node != 0; node = node->next)
		++result;
	return result;
}

struct slist_iterator_base
{
	typedef forward_iterator_tag	iterator_category;
	typedef size_t					size_type;
	typedef ptrdiff_t				differrnce_type;

	slist_node_base*				Node;

	slist_iterator_base(slist_node_base* n) : Node(n) {}

	void incr() { Node = Node->next; }

	bool operator == (slist_iterator_base const& x) const
	{
		return Node == x.Node;
	}

	bool operator != (slist_iterator_base const& x) const
	{
		return !((*this).operator==(x));
	}
};

template <typename T, typename Ref, typename Ptr>
struct slist_iterator : public slist_iterator_base
{
	typedef slist_iterator<T, T&, T*>				iterator;
	typedef slist_iterator<T, T const&, T const*>	const_iterator;
	typedef slist_iterator<T, Ref, Ptr>				self;

	typedef T				value_type;
	typedef Ptr				pointer;
	typedef Ref				reference;
	typedef slist_node<T>	NodeType;

	slist_iterator(NodeType* x) : slist_iterator_base(x) {}
	slist_iterator() : slist_iterator_base(nullptr) {}
	slist_iterator(iterator const& x) : slist_iterator_base(x.Node) {}

	reference operator * () const { return ((NodeType*)Node)->data; }
	pointer operator -> () const { return &((*this).operator*()); }

	self& operator ++ ()
	{
		incr();
		return *this;
	}

	self& operator ++ (int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
};

template <typename T, typename Alloc>
class slist_base
{
public:

	typedef Alloc	allocator_type;
	allocator_type get_allocator() const { return allocator_type(); }

	slist_base(allocator_type const&) { this->Head.next = 0; }
	~slist_base() { erase_after(Head, 0); }

protected:

	typedef simple_alloc<slist_node<T>, Alloc>	node_allocator;

	slist_node<T>* get_node() { return node_allocator::allocate(1); }
	void put_node(slist_node<T>* pNode) { node_allocator::deallocate(pNode, 1); }

	slist_node_base* erase_after(slist_node_base* pos)
	{
		slist_node<T>* next = (slist_node<T>*)(pos->next);
		slist_node_base* next_next = next->next;
		pos->next = next_next;
		Destroy(&next->data);
		put_node(next);
		return next_next;
	}
	slist_node_base* erase_after(slist_node_base*, slist_node_base*);

protected:

	slist_node_base Head;

};

template <typename T, typename Alloc>
slist_node_base* slist_base<T, Alloc>::erase_after(
							slist_node_base* First,
							slist_node_base* Last)
{
	slist_node<T>* Cur = (slist_node<T>*)(First->next);
	while (First != Last)
	{
		slist_node<T>* Tmp = Cur;
		Cur = (slist_node<T>*)(Cur->next);
		Destroy(&Tmp->data);
		put_node(Tmp);
	}
	First->next = Last;
	return Last;
}

template <typename T, typename Alloc = alloc>
class slist : protected slist_base<T, Alloc>
{
private:

	typedef slist_base<T, Alloc>		base;

public:

	typedef T							value_type;
	typedef value_type*					pointer;
	typedef value_type const*			const_pointer;
	typedef value_type&					reference;
	typedef value_type const&			const_reference;
	typedef size_t						size_type;
	typedef ptrdiff_t					difference_type;

	typedef slist_iterator<T, T&, T*>				iterator;
	typedef slist_iterator<T, T const&, T const*>	const_iterator;

	typedef typename base::allocator_type	allocator_type;
	allocator_type get_allocator() const { return base::get_allocator(); }

private:

	typedef slist_node<T>			NodeType;
	typedef slist_node_base			LinkType;

	NodeType* create_node(value_type const& x)
	{
		NodeType* node = get_node();
		Construct(&node->data, x);
		node->next = 0;
		return node;
	}

	NodeType* create_node()
	{
		NodeType* node = get_node();
		Construct(&node->data);
		node->next = 0;
		return node;
	}

public:

	explicit slist(allocator_type const& a = allocator_type()) : base(a) {}
	explicit slist(size_type n)	: base(allocator_type())
	{
		insert_after_fill(&this->Head, n, value_type());
	}

	slist(size_type n, value_type const& x, allocator_type const& a = allocator_type())	: base(a)
	{
		insert_after_fill(&this->Head, n, x);
	}

	slist(slist const& x) : base(x.get_allocator())
	{
		insert_after_range(&this->Head, x.begin(), x.end());
	}
	slist& operator = (slist const& x);
	
	~slist() {}

public:

	iterator begin() { return iterator((NodeType*)this->Head.next); }
	const_iterator begin() const { return const_iterator((NodeType*)this->Head.next); }

	iterator end() { return iterator(0); }
	const_iterator end() const { return const_iterator(0); }

	//iterator before_begin() { return iterator((NodeType*)&this->Head); }
	//const_iterator before_begin() const { return const_iterator((NodeType*)&this->Head); }

	size_type size() const { return slist_size(this->Head.next); }
	bool empty() const { return this->Head.next == 0; }

	void Swap(slist& x)
	{
		Swap(this->Head.next, x.Head.next);
	}

public:

	reference front() { return ((NodeType*)this->Head.next)->data; }
	const_reference front() const {	return return ((NodeType*)this->Head.next)->data; }

	void push_front() { slist_make_link(&this->Head, create_node()); }
	void push_front(value_type const& x) { slist_make_link(&this->Head, create_node(x)); }

	void pop_front() 
	{
		NodeType* node = (NodeType*)this->Head.next;
		this->Head.next = node->next;
		Destroy(&node->data);
		put_node(node);
	}

	iterator previous(const_iterator pos)
	{
		return iterator((NodeType*)slist_previous(&this->Head, pos.Node));
	}

	const_iterator previous(const_iterator pos)
	{
		return const_iterator((NodeType*)slist_previous(&this->Head, pos.Node));
	}

private:

	NodeType* insert_after(LinkType* pos, value_type const& x)
	{
		return (NodeType*)slist_make_link(pos, create_node(x));
	}

	NodeType* insert_after(LinkType* pos)
	{
		return (NodeType*)slist_make_link(pos, create_node());
	}

	void insert_after_fill(LinkType* pos, size_type n, value_type const& x)
	{
		for (size_type i = 0; i < n; ++i)
			pos = slist_make_link(pos, create_node(x));
	}

	void insert_after_range(LinkType* pos, const_iterator First, const_iterator Last)
	{
		while (First != Last)
		{
			pos = slist_make_link(pos, create_node(*First));
			++First;
		}
	}

	void insert_after_range(LinkType* pos, value_type const* First, value_type const* Last)
	{
		while (First != Last)
		{
			pos = slist_make_link(pos, create_node(*First));
			++First;
		}
	}

public:

	iterator insert_after(iterator pos, value_type const& x)
	{
		return iterator(insert_after(pos.Node, x));
	}

	iterator insert_after(iterator pos)
	{
		return iterator(insert_after(pos.Node));
	}

	void insert_after(iterator pos, size_type n, value_type const& x)
	{
		insert_after_fill(pos.Node, n, x);
	}

	void insert_after(iterator pos, const_iterator First, const_iterator Last)
	{
		insert_after_range(pos.Node, First, Last);
	}

	void insert_after(iterator pos, value_type const* First, value_type const* Last)
	{
		insert_after_range(pos.Node, First, Last);
	}

	iterator insert(iterator pos, value_type const& x)
	{
		return iterator(insert_after(previous(pos).Node, x));
	}

	iterator insert(iterator pos)
	{
		return iterator(insert_after(previous(pos).Node));
	}

	void insert(iterator pos, size_type n, value_type const& x)
	{
		insert_after_fill(previous(pos).Node, n, x);
	}

	void insert(iterator pos, const_iterator First, const_iterator Last)
	{
		insert_after_range(previous(pos).Node, First, Last);
	}

	void insert(iterator pos, value_type const* First, value_type const* Last)
	{
		insert_after_range(previous(pos).Node, First, Last);
	}

	iterator erase_after(iterator pos) 
	{
		return iterator((NodeType*)this->erase_after(pos.Node));
	}

	iterator erase_after(iterator BeforeFirstNode, iterator Last)
	{
		return iterator((NodeType*)this->erase_after(BeforeFirstNode.Node,	Last.Node));
	}

	iterator erase(iterator pos)
	{
		return iterator((NodeType*)this->erase_after(previous(pos).Node));
	}

	iterator erase(iterator First, iterator Last)
	{
		return iterator((NodeType*)this->erase_after(previous(First).Node, Last.Node));
	}

	void clear() { this->erase_after(&this->Head, 0); }

	void splice_after(iterator pos, iterator BeforeFirstNode, iterator BeforeLastNode)
	{
		if (BeforeFirstNode != BeforeLastNode)
			slist_splice_after(pos.Node, BeforeFirstNode.Node, BeforeLastNode.Node);
	}

	void splice_after(iterator pos, slist& x)
	{
		slist_splice_after(pos.Node, &x.Head);
	}

	void splice(iterator pos, slist& x, iterator First, iterator Last)
	{
		if (First != Last)
			slist_splice_after(previous(pos).Node, 
							slist_previous(&x.Head, First).Node,
							slist_previous(First.Node, Last).Node);
	}

	void splice(iterator pos, slist& x)
	{
		slist_splice_after(previous(pos).Node, &x.Head);
	}

public:

	void reverse()
	{
		if (this->Head.next)
			this->Head.next = slist_reverse(this->Head.next);
	}
	void remove(T const& Value);
	void unique();
	void merge(slist& x);
	void sort();
};

template <typename T, typename Alloc>
slist<T, Alloc>& slist<T, Alloc>::operator=(slist<T, Alloc> const& x)
{
	if (&x != this)
	{
		LinkType* link = &this->Head;
		NodeType* pNode = (NodeType*)this->Head.next;
		NodeType const* pCopiedNode = (NodeType const*)x.Head.next;

		while (pNode && pCopiedNode)
		{
			pNode->data = pCopiedNode->data;
			link = pNode;
			pNode = (NodeType*)pNode->next;
			pCopiedNode = (NodeType const*)pNode->next;
		}
		if (pCopiedNode == 0)
			this->erase_after(link, 0);
		else
			insert_after_range(link, const_iterator((NodeType*)pCopiedNode), const_iterator(0));
	}
	return *this;
}

template <typename T, typename Alloc>
void slist<T, Alloc>::remove(T const& Value)
{
	LinkType* Cur = &this->Head;
	while (Cur && Cur->next) 
	{
		if (((NodeType*)Cur->next)->data == Value)
			this->erase_after(Cur);
		else
			Cur = Cur->next;
	}
}

template <typename T, typename Alloc>
void slist<T, Alloc>::unique()
{
	LinkType* Cur = &this->Head.next;
	if (Cur)
	{
		while (Cur->next) {
			if (((NodeType*)Cur)->data == ((NodeType*)(Cur->next))->data)
				this->erase_after(Cur);
			else
				Cur = Cur->next;
		}
	}
}

template <typename T, typename Alloc>
void slist<T, Alloc>::merge(slist& x)
{
	LinkType* link = &this->Head;
	while (link->next && x.Head.next)
	{
		if (((NodeType*)x.Head.next)->data < ((NodeType*)link->next)->data)
			slist_splice_after(link, &x.Head, x.Head.next);
		link = link->next;
	}

	if (x.Head.next)
	{
		link->next = x.Head.next;
		x.Head.next = 0;
	}
}

template <typename T, typename Alloc>
void slist<T, Alloc>::sort()
{
	if (this->Head.next && this->Head.next->next) {
		slist carry;
		slist counter[64];
		int fill = 0;

		while (!empty())
		{
			slist_splice_after(&carry.Head, &this->Head, this->Head.next);
			int i = 0;
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i]);
				++i;
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}

		for (int i = 1; i < fill; ++i)
			counter[i].merge(counter[i - 1]);
		this->swap(counter[fill - 1]);
	}eeeee
}