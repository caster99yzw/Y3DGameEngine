#pragma once

#include "SGISTL/IteratorBase.h"
#include "Template/Template.h"
#include "CoreTypes.h"

typedef bool	RBTreeColorType;
RBTreeColorType const RBTreeNodeRed		= false;
RBTreeColorType const RBTreeNodeBlack	= true;

struct RBTreeNodeBase
{
	typedef RBTreeColorType		ColorType;
	typedef RBTreeNodeBase*		BasePointer;

	ColorType		Color;
	BasePointer		Parent;
	BasePointer		Left;
	BasePointer		Right;

	static BasePointer Minimum(BasePointer x)
	{
		while (x->Left != 0)
			x = x->Left;
		return x;
	}

	static BasePointer Maximum(BasePointer x)
	{
		while (x->Right != 0)
			x = x->Right;
		return x;
	}
};

template <typename NodeValueType>
struct RBTreeNode : public RBTreeNodeBase
{
	typedef RBTreeNode<NodeValueType>*	LinkType;
	NodeValueType	Vaule_field;
};

FORCEINLINE void RBTreeRotateLeft(RBTreeNodeBase* Cur, RBTreeNodeBase*& Root)
{
	RBTreeNodeBase* NewCur = Cur->Right;
	Cur->Right = NewCur->Left;
	if (NewCur->Left != 0)
		NewCur->Left->Parent = Cur;

	NewCur->Parent = Cur->Parent;

	if (Cur == Root)
		Root = NewCur;
	else if (Cur == Cur->Parent->Left)
		Cur->Parent->Left = NewCur;
	else
		Cur->Parent->Right = NewCur;

	NewCur->Left = Cur;
	Cur->Parent = NewCur;
}

FORCEINLINE void RBTreeRotateRight(RBTreeNodeBase* Cur, RBTreeNodeBase*& Root)
{
	RBTreeNodeBase* NewCur = Cur->Left;
	Cur->Left = NewCur->Right;
	if (NewCur->Right != 0)
		NewCur->Right->Parent = Cur;

	NewCur->Parent = Cur->Parent;

	if (Cur == Root)
		Root = NewCur;
	else if (Cur == Cur->Parent->Left)
		Cur->Parent->Left = NewCur;
	else
		Cur->Parent->Right = NewCur;

	NewCur->Right = Cur;
	Cur->Parent = NewCur;
}

FORCEINLINE void RBTreeRebalance(RBTreeNodeBase* Cur, RBTreeNodeBase* Root)
{
	Cur->Color = RBTreeNodeRed;

	while (Cur != Root && Cur->Parent->Color == RBTreeNodeRed)
	{
		if (Cur->Parent == Cur->Parent->Parent->Left)
		{
			RBTreeNodeBase* Uncle = Cur->Parent->Parent->Right;
			if (Uncle && Uncle->Color == RBTreeNodeRed)
			{
				Cur->Parent->Color = RBTreeNodeBlack;
				Uncle->Color = RBTreeNodeBlack;
				Cur->Parent->Parent->Color = RBTreeNodeRed;
				Cur = Cur->Parent->Parent;
			}
			else
			{
				if (Cur == Cur->Parent->Right)
				{
					Cur = Cur->Parent;
					RBTreeRotateLeft(Cur, Root);
				}
				Cur->Parent->Color = RBTreeNodeBlack;
				Cur->Parent->Parent->Color = RBTreeNodeRed;
				RBTreeRotateRight(Cur->Parent->Parent, Root);
			}
		}
		else
		{
			RBTreeNodeBase* Uncle = Cur->Parent->Parent->Left;
			if (Uncle && Uncle->Color == RBTreeNodeRed)
			{
				Cur->Parent->Color = RBTreeNodeBlack;
				Uncle->Color = RBTreeNodeBlack;
				Cur->Parent->Parent->Color = RBTreeNodeRed;
				Cur = Cur->Parent->Parent;
			}
			else
			{
				if (Cur == Cur->Parent->Left)
				{
					Cur = Cur->Parent;
					RBTreeRotateRight(Cur, Root);
				}
				Cur->Parent->Color = RBTreeNodeBlack;
				Cur->Parent->Parent->Color = RBTreeNodeRed;
				RBTreeRotateLeft(Cur->Parent->Parent, Root);
			}

		}
	}
	Root->Color = RBTreeNodeBlack;
}

FORCEINLINE RBTreeNodeBase* RBTreeRebalanceForErase(RBTreeNodeBase* CurNode,
													RBTreeNodeBase* Root,
													RBTreeNodeBase* MinimumNode,
													RBTreeNodeBase* MaximumNode)
{
	RBTreeNodeBase* DeletedNode = CurNode;
	RBTreeNodeBase* ReBalanceNode = 0;
	RBTreeNodeBase* ReBalanceParentNode = 0;

	// Erase
	if (DeletedNode->Left == 0)
	{
		ReBalanceNode = DeletedNode->Right;
	}
	else
	{
		if (DeletedNode->Right == 0)
		{
			ReBalanceNode = DeletedNode->Left;
		}
		else
		{
			DeletedNode = DeletedNode->Right;
			while (DeletedNode != 0)
			{
				DeletedNode = DeletedNode->Left;
			}
			DeletedNode = DeletedNode->Right;
		}
	}

	if (DeletedNode != CurNode)
	{
		CurNode->Left->Parent = DeletedNode;

		DeletedNode->Left = CurNode->Left;
		if (DeletedNode != CurNode->Right)
		{
			ReBalanceParentNode = DeletedNode->Parent;
			if (ReBalanceNode) ReBalanceNode->Parent = DeletedNode->Parent;
			DeletedNode->Parent->Left = ReBalanceNode;
			DeletedNode->Right = CurNode->Right;
			CurNode->Right->Parent = DeletedNode;
		}
		else
		{
			ReBalanceParentNode = DeletedNode;
		}
		if (Root == CurNode)
			Root = DeletedNode;
		if (CurNode->Parent->Left == CurNode)
			CurNode->Parent->Left = DeletedNode;
		else
			CurNode->Parent->Right = DeletedNode;
		DeletedNode->Parent = CurNode->Parent;
		::Swap(DeletedNode->Color, CurNode->Color);
		DeletedNode = CurNode;
	}
	else
	{
		ReBalanceParentNode = DeletedNode->Parent;
		if (ReBalanceNode) ReBalanceNode->Parent = DeletedNode->Parent;
		if (Root == CurNode)
		{
			Root = ReBalanceNode;
		}
		else
		{
			if (CurNode->Parent->Left == CurNode)
				CurNode->Parent->Left = ReBalanceNode;
			else
				CurNode->Parent->Right = ReBalanceNode;
		}
		if (MinimumNode == CurNode)
		{
			if (CurNode->Right == 0)
				MinimumNode = CurNode->Parent;
			else
				MinimumNode = RBTreeNodeBase::Minimum(ReBalanceNode);
		}
		if (MaximumNode == CurNode)
		{
			if (CurNode->Left == 0)
				MaximumNode = CurNode->Parent;
			else
				MaximumNode = RBTreeNodeBase::Minimum(ReBalanceNode);
		}
	}

	// Re Balance
	if (DeletedNode->Color != RBTreeNodeRed)
	{
		while (ReBalanceNode != Root && (ReBalanceNode == 0 || ReBalanceNode->Color == RBTreeNodeBlack))
		{
			if (ReBalanceNode == ReBalanceParentNode->Left)
			{
				RBTreeNodeBase* ReBalanceUncleNode = ReBalanceParentNode->Right;
				if (ReBalanceUncleNode->Color == RBTreeNodeRed)
				{
					ReBalanceUncleNode->Color = RBTreeNodeBlack;
					ReBalanceParentNode->Color = RBTreeNodeRed;
					RBTreeRotateLeft(ReBalanceParentNode, Root);
					ReBalanceUncleNode = ReBalanceParentNode->Right;
				}
				if ((ReBalanceUncleNode->Left == 0 || ReBalanceUncleNode->Left->Color == RBTreeNodeBlack) &&
					(ReBalanceUncleNode->Right == 0 || ReBalanceUncleNode->Right->Color == RBTreeNodeBlack))
				{
					ReBalanceUncleNode->Color = RBTreeNodeRed;
					ReBalanceNode = ReBalanceParentNode;
					ReBalanceParentNode = ReBalanceParentNode->Parent;
				}
				else
				{
					if (ReBalanceUncleNode->Right == 0 || ReBalanceUncleNode->Right->Color == RBTreeNodeBlack)
					{
						if (ReBalanceUncleNode->Left) ReBalanceUncleNode->Left->Color = RBTreeNodeBlack;
						ReBalanceUncleNode->Color = RBTreeNodeRed;
						RBTreeRotateRight(ReBalanceUncleNode, Root);
						ReBalanceUncleNode = ReBalanceParentNode->Right;
					}
					ReBalanceUncleNode->Color = ReBalanceParentNode->Color;
					ReBalanceParentNode->Color = RBTreeNodeBlack;
					if (ReBalanceUncleNode->Right) ReBalanceUncleNode->Right->Color = RBTreeNodeBlack;
					RBTreeRotateLeft(ReBalanceParentNode, Root);
					break;
				}
			}
			else
			{
				RBTreeNodeBase* ReBalanceUncleNode = ReBalanceParentNode->Left;
				if (ReBalanceUncleNode->Color == RBTreeNodeRed)
				{
					ReBalanceUncleNode->Color = RBTreeNodeBlack;
					ReBalanceParentNode->Color = RBTreeNodeRed;
					RBTreeRotateRight(ReBalanceParentNode, Root);
					ReBalanceUncleNode = ReBalanceParentNode->Left;
				}
				if ((ReBalanceUncleNode->Right == 0 || ReBalanceUncleNode->Right->Color == RBTreeNodeBlack) &&
					(ReBalanceUncleNode->Left == 0 || ReBalanceUncleNode->Left->Color == RBTreeNodeBlack))
				{
					ReBalanceUncleNode->Color = RBTreeNodeRed;
					ReBalanceNode = ReBalanceParentNode;
					ReBalanceParentNode = ReBalanceParentNode->Parent;
				}
				else
				{
					if (ReBalanceUncleNode->Left == 0 || ReBalanceUncleNode->Left->Color == RBTreeNodeBlack)
					{
						if (ReBalanceUncleNode->Right) ReBalanceUncleNode->Right->Color = RBTreeNodeBlack;
						ReBalanceUncleNode->Color = RBTreeNodeRed;
						RBTreeRotateLeft(ReBalanceUncleNode, Root);
						ReBalanceUncleNode = ReBalanceParentNode->Left;
					}
					ReBalanceUncleNode->Color = ReBalanceParentNode->Color;
					ReBalanceParentNode->Color = RBTreeNodeBlack;
					if (ReBalanceUncleNode->Left) ReBalanceUncleNode->Left->Color = RBTreeNodeBlack;
					RBTreeRotateRight(ReBalanceParentNode, Root);
					break;
				}
			}
		}
		if (ReBalanceNode) ReBalanceNode->Color = RBTreeNodeBlack;
	}
	return DeletedNode;
}

struct RBTreeBaseIterator
{
	typedef RBTreeNodeBase::BasePointer		BasePointer;
	typedef bidirectional_iterator_tag		iterator_category;
	typedef ptrdiff_t						difference_type;

	BasePointer		Node;

	void increment()
	{
		if (Node->Right != 0)
		{
			Node = Node->Right;
			while (Node->Left != 0)
				Node = Node->Left;
		}
		else
		{
			BasePointer Parent = Node->Parent;
			while (Node = Parent->Right)
			{
				Node = Parent;
				Parent = Parent->Parent;
			}

			if (Node->Right != Parent)
				Node = Parent;
		}
	}

	void decrement()
	{
		if (Node->Color == RBTreeNodeRed && Node->Parent->Parent == Node)
		{
			Node = Node->Right;
		}
		else if (Node->Left != 0)
		{
			Node = Node->Left;
			while (Node->Right != 0)
				Node = Node->Right;
		}
		else
		{
			BasePointer Parent = Node->Parent;
			while (Node = Parent->Left)
			{
				Node = Parent;
				Parent = Parent->Parent;
			}
			Node = Parent;
		}
	}
};

template <typename NodeValueType, typename Ref, typename Ptr>
struct RBTreeIterator : public RBTreeBaseIterator
{
	typedef NodeValueType					ValueType;
	typedef Ref							Reference;
	typedef Ptr							Pointer;
	typedef RBTreeNode<NodeValueType>*		LinkType;

	typedef RBTreeIterator<NodeValueType, Ref, Ptr>									self;
	typedef RBTreeIterator<NodeValueType, NodeValueType&, NodeValueType*>				iterator;
	typedef RBTreeIterator<NodeValueType, NodeValueType const&, NodeValueType const*>	const_iterator;

	RBTreeIterator() {}
	RBTreeIterator(LinkType x) { Node = x; }
	RBTreeIterator(iterator const& other) { Node = other.Node; }

	Reference operator * () { return LinkType(Node).Vaule_field; }
	Reference operator -> () { return &(this->operator*()); }

	self& operator ++ () 
	{
		increment();
		return *this;
	}
	self operator ++ (int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator -- ()
	{
		decrement();
		return *this;
	}
	self operator -- (int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}
};

FORCEINLINE bool operator == (RBTreeBaseIterator const& lhs, RBTreeBaseIterator const& rhs)
{
	return lhs.Node == rhs.Node;
}

FORCEINLINE bool operator != (RBTreeBaseIterator const& lhs, RBTreeBaseIterator const& rhs)
{
	return lhs.Node != rhs.Node;
}

template <typename NodeValueType, typename Alloc>
struct RBTreeBase
{
	typedef RBTreeNode<NodeValueType>		NodeType;
	typedef RBTreeNode<NodeValueType>*		LinkType;
	typedef Alloc							allocator_type;

	allocator_type	get_allocator() const { return allocator_type(); }

	RBTreeBase(const allocator_type&) { Header = get_node(); }
	~RBTreeBase() { put_node(Header); }

protected:

	LinkType	Header;

protected:

	typedef simple_alloc<NodeType, Alloc> node_allocator;

	LinkType get_node()
	{
		return node_allocator::allocate(1);
	}

	void put_node(LinkType p)
	{
		node_allocator::deallocate(p, 1);
	}
};

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc = alloc>
struct RBTree : public RBTreeBase<NodeValueType, Alloc>
{
	typedef RBTreeBase<NodeValueType, Alloc>	base;

protected:

	typedef RBTreeNodeBase::BasePointer		BasePointer;
	typedef RBTreeColorType					Colortype;

public:

	typedef Key								KeyType;
	typedef NodeValueType					ValueType;
	typedef NodeValueType*					pointer;
	typedef NodeValueType const*			const_pointer;
	typedef NodeValueType&					reference;
	typedef NodeValueType const&			const_reference;
	typedef size_t							size_type;
	typedef ptrdiff_t						difference_type;

protected:

	LinkType create_node(ValueType const& x)
	{
		LinkType tmp = get_node();
		Construct(&tmp->Vaule_field, x);
		return tmp;
	}

	LinkType clone_node(LinkType x)
	{
		LinkType tmp = create_node(x->Vaule_field);
		tmp->Color = x->Color;
		tmp->Left = 0;
		tmp->Right = 0;
		return tmp;
	}

	void destory_node(LinkType p)
	{
		Destroy(&p->Vaule_field);
		put_node(p);
	}

protected:

	size_type		NodeCount;
	CompareType		KeyCompare;

	LinkType& root() const { return (LinkType&)(Header->Parent); }
	LinkType& leftmost() const { return (LinkType&)(Header->Left); }
	LinkType& rightmost() const { return (LinkType&)(Header->Right); }

	static LinkType& left(LinkType x) { return (LinkType&)(x->Left); }
	static LinkType& right(LinkType x) { return (LinkType&)(x->Right); }
	static LinkType& parent(LinkType x) { return (LinkType&)(x->Parent); }

	static reference value(LinkType x) { return x->Vaule_field; }
	static KeyType const& key(LinkType x) { return KeyOfValue()(value(x)); }
	static LinkType& color(LinkType x) { return (Colortype&)(x->Color); }

	static LinkType& left(BasePointer x) { return (LinkType&)(x->Left); }
	static LinkType& right(BasePointer x) { return (LinkType&)(x->Right); }
	static LinkType& parent(BasePointer x) { return (LinkType&)(x->Parent); }

	static reference value(BasePointer x) { return (LinkType)x->Vaule_field; }
	static KeyType const& key(BasePointer x) { return KeyOfValue()(value(LinkType(x))); }
	static LinkType& color(BasePointer x) { return (Colortype&)((LinkType)x->Color); }

	static LinkType& minimum(LinkType x)
	{
		return (LinkType)RBTreeNodeBase::Minimum(x);
	}
	static LinkType& maximum(LinkType x)
	{
		return (LinkType)RBTreeNodeBase::Maximum(x);
	}

public:

	typedef RBTreeIterator<ValueType, reference, pointer>				iterator;
	typedef RBTreeIterator<ValueType, const_reference, const_pointer>	const_iterator;

private:

	iterator insert_aux(BasePointer CurPtr, BasePointer ParentPtr, ValueType const& Value);
	LinkType copy_aux(LinkType RootNode, LinkType CopiedRootParentNode);

	void initialize()
	{
		Header = get_node();
		Header->Color = RBTreeNodeRed;

		root() = 0;
		leftmost() = Header;
		rightmost() = Header;
	}

public:

	RBTree() 
		: base(allocator_type()), NodeCount(0), KeyCompare()
	{
		initialize();
	}

	RBTree(CompareType const& Comp)
		: base(allocator_type()), NodeCount(0), KeyCompare(Comp)
	{
		initialize();
	}

	RBTree(CompareType const& Comp, allocator_type const& a)
		: base(a), NodeCount(0), KeyCompare(Comp)
	{
		initialize();
	}

	RBTree(RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc> const& x)
		: base(x.get_allocator()), NodeCount(0), KeyCompare(x.KeyCompare)
	{
		if (x.root() == 0)
		{
			initialize();
		}
		else 
		{
			color(Header) = RBTreeNodeRed;
			root() = copy_aux(x.root(), Header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
			NodeCount = x.NodeCount;
		}
	}
	~RBTree() { clear(); }

	RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>&	
		operator = (const RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>& __x);

public:

	CompareType key_compare() const { return KeyCompare; }

	iterator begin() { return leftmost(); }
	const_iterator begin() const { return leftmost(); }
	iterator end() { return Header; }
	const_iterator end() const { return Header; }

	bool empty() const { return NodeCount == 0; }
	size_type size() const { return NodeCount; }

	void Swap(RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>& other)
	{
		::Swap(Header, other.Header);
		::Swap(NodeCount, other.NodeCount);
		::Swap(KeyCompare, other.KeyCompare);
	}

	std::pair<iterator, bool> insert_unique(ValueType const& Value);
	iterator insert_equal(ValueType const& Value);

	void insert_unique(const_iterator First, const_iterator Last);
	void insert_unique(ValueType const* First, ValueType const* Last);
	void insert_equal(const_iterator First, const_iterator Last);
	void insert_equal(ValueType const* First, ValueType const* Last);

	void erase(iterator Pos);
	size_type erase(KeyType const& x);

	void erase(iterator First, iterator Last);
	void erase(KeyType const* First, KeyType const* Last);

	void erase_all(LinkType x);
	void clear() 
	{
		if (NodeCount != 0) 
		{
			erase_all(root());
			leftmost() = Header;
			root() = 0;
			rightmost() = Header;
			NodeCount = 0;
		}
	}

public:

	iterator find(KeyType const& x);
	const_iterator find(KeyType const& x) const;

	size_type count(KeyType const& x) const;

	iterator lower_bound(KeyType const& x);
	const_iterator lower_bound(KeyType const& x) const;
	iterator upper_bound(KeyType const& x);
	const_iterator upper_bound(KeyType const& x) const;

	std::pair<iterator, iterator> equal_range(KeyType const& x);
	std::pair<const_iterator, const_iterator> equal_range(KeyType const& x) const;
};

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::LinkType
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::copy_aux(LinkType RootNode, LinkType CopiedRootParentNode)
{
	LinkType CopiedRootNode = clone_node(RootNode);
	CopiedRootNode->Parent = CopiedRootParentNode;

	if (RootNode->Right)
	{
		CopiedRootNode->Right = copy_aux(right(RootNode), CopiedRootNode);
	}
	CopiedRootParentNode = CopiedRootNode;
	RootNode = left(RootNode);

	while (x != 0)
	{
		LinkType CopiedLeftRootNode = clone_node(RootNode);
		CopiedRootParentNode->Left = CopiedLeftRootNode;
		CopiedLeftRootNode->Parent = CopiedRootParentNode;
		if (RootNode->Right)
		{
			CopiedLeftRootNode->Right = copy_aux(right(RootNode), CopiedLeftRootNode);
		}
		CopiedRootParentNode = y;
		RootNode = left(RootNode);
	}
	return CopiedRootNode;
}


template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>&
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::operator = (RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc> const& x)
{
	if (this != &x)
	{
		clear();
		NodeCount = 0;
		KeyCompare = x.KeyCompare;
		if (x.root() == 0)
		{
			root() = 0;
			leftmost() = Header;
			rightmost() = Header;
		}
		else 
		{
			root() = copy_aux(x.root(), Header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
			NodeCount = x.NodeCount;
		}
	}
	return *this;
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_equal(ValueType const& Value)
{
	LinkType Parent = Header;
	LinkType Cur = root();

	while (Cur != 0)
	{
		Parent = Cur;
		Cur = key_compare(KeyOfValue()(Value), key(Cur)) ? left(Cur) : right(Cur);
	}
	return insert_aux(Cur, Parent, Value);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
std::pair<typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator, bool>
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_unique(ValueType const& Value)
{
	LinkType Parent = Header;
	LinkType Cur = root();

	bool CompResult = true;
	while (Cur != 0)
	{
		Parent = Cur;
		CompResult = key_compare(KeyOfValue()(Value), key(Cur));
		Cur = CompResult ? left(Cur) : right(Cur);
	}

	/*
	* If compare type is Less
	* when CompResult is true, means Value is must less than Parent of Parent node 
	* when CompResult is false, means Value is may greater or equal than Parent of Parent node 
	*/
	iterator ParentPtr = iterator(Parent);
	if (CompResult)
	{
		if (ParentPtr == begin())
			return std::pair<iterator, bool>(insert_aux(Cur, Parent, Value), false);
		else
			--ParentPtr;
	}

	if (key_compare(key(ParentPtr.Node), KeyOfValue()(Value)))
	{
		return std::pair<iterator, bool>(insert_aux(Cur, Parent, Value), false);
	}

	return std::pair<iterator, bool>(ParentPtr, false);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_aux(BasePointer CurPtr, BasePointer ParentPtr, ValueType const& Value)
{
	LinkType Parent = (LinkType)ParentPtr;
	LinkType Cur = (LinkType)CurPtr;
	LinkType InsertPosition;

	if (Parent == Header || Cur != 0 || key_compare(KeyOfValue()(Value), key(Parent)))
	{
		InsertPosition = create_node(Value);
		left(Parent) = InsertPosition;
		if (Parent == Header)
		{
			root() = InsertPosition;
			rightmost() = InsertPosition;
		}
		else if (Parent == leftmost())
		{
			leftmost() = InsertPosition;
		}
	}
	else
	{
		InsertPosition = create_node(Value);
		right(Parent) = InsertPosition;
		if (Parent == rightmost())
		{
			rightmost() = InsertPosition;
		}
	}
	parent(InsertPosition) = Parent;
	left(InsertPosition) = 0;
	right(InsertPosition) = 0;

	RBTreeRebalance(InsertPosition, Header->Parent);
	++NodeCount;
	return iterator(InsertPosition);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_equal(ValueType const* First, ValueType const* Last)
{
	for (; First != Last; ++First)
		insert_equal(*First);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_equal(const_iterator First, const_iterator Last)
{
	for (; First != Last; ++First)
		insert_equal(*First);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_unique(ValueType const* First, ValueType const* Last)
{
	for (; First != Last; ++First)
		insert_unique(*First);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::insert_unique(const_iterator First, const_iterator Last)
{
	for (; First != Last; ++First)
		insert_unique(*First);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::erase(iterator Pos)
{
	LinkType DeletedNode = (LinkType)RBTreeRebalanceForErase(Pos.Node, Header->Parent, Header->Left, Header->Right);
	destory_node(DeletedNode);
	--NodeCount;
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::size_type
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::erase(KeyType const& x)
{
	pair<iterator, iterator> Iterator_Range = equal_range(x);
	size_type ResultCount = 0;
	distance(Iterator_Range.first, Iterator_Range.second, ResultCount);
	erase(Iterator_Range.first, Iterator_Range.second);
	return ResultCount;
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::erase_all(LinkType Root)
{
	while (Root != 0) 
	{
		erase_all(right(Root));
		LinkType LeftChild = left(Root);
		destory_node(Root);
		Root = LeftChild;
	}
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::erase(iterator First, iterator Last)
{
	if (First == begin() && Last == end())
		clear();
	else
		while (First != Last) erase(First++);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
void RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::erase(KeyType const* First, KeyType const* Last)
{
	while (First != Last) 
		erase(*First++);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::find(KeyType const& x) 
{
	iterator Result = lower_bound(x);
	return (Result == end() || key_compare(x, key(Result.Node)) ? end() : Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::const_iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::find(KeyType const& x) const
{
	iterator Result = lower_bound(x);
	return (Result == end() || key_compare(x, key(Result.Node)) ? end() : Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::size_type
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::count(KeyType const& x) const
{
	std::pair<const_iterator, const_iterator> Iterator_Range = equal_range(x);
	size_type n = 0;
	distance(Iterator_Range.first, Iterator_Range.second, n);
	return n;
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::lower_bound(KeyType const& x)
{
	LinkType Result = Header;
	LinkType Cur = root();

	while (Root != 0)
	{
		if (!key_compare(key(Cur), x))
		{
			Result = Cur;
			Cur = left(Cur);
		}
		else
		{
			Cur = right(Cur);
		}
	}
	return iterator(Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::const_iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::lower_bound(KeyType const& x) const
{
	LinkType Result = Header;
	LinkType Cur = root();

	while (Root != 0)
	{
		if (!key_compare(key(Cur), x))
		{
			Result = Cur;
			Cur = left(Cur);
		}
		else
		{
			Cur = right(Cur);
		}
	}
	return const_iterator(Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::upper_bound(KeyType const& x)
{
	LinkType Result	= Header; 
	LinkType Cur	= root(); 

	while (Root != 0)
	{
		if (key_compare(x, key(Cur)))
		{
			Result = Cur;
			Cur = left(Cur);
		}
		else
		{
			Cur = right(Cur);
		}
	}
	return iterator(Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::const_iterator
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::upper_bound(KeyType const& x) const
{
	LinkType Result = Header;
	LinkType Cur = root();

	while (Root != 0)
	{
		if (key_compare(x, key(Cur)))
		{
			Result = Cur;
			Cur = left(Cur);
		}
		else
		{
			Cur = right(Cur);
		}
	}
	return const_iterator(Result);
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
std::pair<typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator,
		  typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::iterator>
	RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::equal_range(KeyType const& x)
{
	return std::pair<iterator, iterator>(lower_bound(x), upper_bound(x));
}

template <typename	Key, typename NodeValueType, typename KeyOfValue, typename CompareType, typename Alloc>
std::pair<typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::const_iterator,
		  typename RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::const_iterator>
RBTree<Key, NodeValueType, KeyOfValue, CompareType, Alloc>::equal_range(KeyType const& x) const
{
	return std::pair<const_iterator, const_iterator>(lower_bound(x), upper_bound(x));
}
