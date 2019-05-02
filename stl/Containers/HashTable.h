#pragma once
#include "Template/TypeHash.h"

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
struct HashTableConstIterator;

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
class HashTable;

template <typename ValueType>
struct HashTableNode
{
	HashTableNode*	Next;
	ValueType		Value;
};

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
struct HashTableIterator
{
	typedef HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>		iterator;
	typedef HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>	const_iterator;

	typedef HashTableNode<ValueType>	Node;
	typedef forward_iterator_tag		iterator_category;
	typedef ValueType					value_type;
	typedef ValueType&					reference;
	typedef ValueType*					pointer;
	typedef ptrdiff_t					difference_type;
	typedef size_t						size_type;

	Node*		Cur;
	HashTable*	Table;

	HashTableIterator(Node* node, HashTable* table) : Cur(node), Table(table) {}
	HashTableIterator() {}

	reference operator * () const { return Cur->Value; }
	pointer operator -> () const { return &(this->operator*()); }
	iterator& operator ++ ();
	iterator operator ++ (int);
	bool operator == (iterator const& iter) const { return Cur == iter->Cur; }
	bool operator != (iterator const& iter) const { return Cur != iter->Cur; }
};

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>&
HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::operator++()
{
	Node const* Old = Cur;
	Cur = Cur->Next;
	if (!Cur)
	{
		//size_type bucket = 
	}
	return *this;
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>
HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::operator++(int)
{
	iterator tmp = *this;
	++*this;
	return tmp;
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
struct HashTableConstIterator
{
	typedef HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>		iterator;
	typedef HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>	const_iterator;

	typedef HashTableNode<ValueType>	Node;
	typedef forward_iterator_tag		iterator_category;
	typedef ValueType					value_type;
	typedef ValueType const&			reference;
	typedef ValueType const*			pointer;
	typedef ptrdiff_t					difference_type;
	typedef size_t						size_type;

	Node const*			Cur;
	HashTable const*	Table;

	HashTableConstIterator(Node const* node, HashTable const* table) : Cur(node), Table(table) {}
	HashTableConstIterator() {}
	HashTableConstIterator(HashTableIterator const& other) : Cur(other.Node), Table(other.Value) {}

	reference operator * () const { return Cur->Value; }
	pointer operator -> () const { return &(this->operator*()); }
	const_iterator& operator ++ ();
	const_iterator operator ++ (int);
	bool operator == (const_iterator const& iter) const { return Cur == iter->Cur; }
	bool operator != (const_iterator const& iter) const { return Cur != iter->Cur; }
};

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>&
HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::operator++()
{
	Node const* Old = Cur;
	Cur = Cur->Next;
	if (!Cur)
	{
		//size_type bucket = 
	}
	return *this;
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>
HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::operator++(int)
{
	const_iterator tmp = *this;
	++*this;
	return tmp;
}

//// Note: assumes long is at least 32 bits.
//enum { __stl_num_primes = 28 };
//
//static const unsigned long __stl_prime_list[__stl_num_primes] =
//{
//	53ul,         97ul,         193ul,       389ul,       769ul,
//	1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
//	49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
//	1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
//	50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
//	1610612741ul, 3221225473ul, 4294967291ul
//};
//
//inline unsigned long __stl_next_prime(unsigned long __n)
//{
//	const unsigned long* __first = __stl_prime_list;
//	const unsigned long* __last = __stl_prime_list + (int)__stl_num_primes;
//	const unsigned long* pos = lower_bound(__first, __last, __n);
//	return pos == __last ? *(__last - 1) : *pos;
//}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
class HashTable
{
public:
	typedef ValueType*				pointer;
	typedef ValueType const*		const_pointer;
	typedef ValueType&				reference;
	typedef ValueType const&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

	typedef HashTableIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>			iterator;
	typedef HashTableConstIterator<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>		const_iterator;

private:

	typedef HashTableNode<ValueType>	Node;
	typedef simple_alloc<Node, Alloc>	node_allocator;

	HashFunction			HashFunc;
	EqualKeyType			EqualKey;
	ExtractKeyType			ExtractKey;
		
	Array<Node*, Alloc>	Buckets;
	size_type				ElementsNum;

	Node* new_node(ValueType const& Value)
	{
		Node* tmpNode = node_allocator::allocate();
		tmpNode->Next = 0;
		Construct(&tmpNode->Value, Value);
		return tmpNode;
	}

	void delete_node(Node* node)
	{
		Destroy(&node->Value);
		node_allocator::deallocate(node);
	}

public:

	HashTable(size_type n, HashFunction const& hf, HashFunction const& eql)
		: HashFunc(hf), EqualKey(eql), ExtractKey(ExtractKeyType()), ElementsNum(n)
	{
		InitializeBuckets();
	}

private:

	size_type next_size(size_type n) const
	{
		// ??????????
		return 0;
	}

	void InitializeBuckets(size_type n)
	{
		size_type const BucketsNum = next_size(n);
		Buckets.reserve(BucketsNum);
		Buckets.insert(Buckets.end(), BucketsNum, (Node*)0);
		ElementsNum = 0;
	}

	size_type GetKeyTypeHash(KeyType const& Key) const
	{
		return GetKeyTypeHash(Key, Buckets.size());
	}

	size_type GetValueTypeHash(ValueType const& Value) const
	{
		return GetKeyTypeHash(ExtractKey(Value));
	}

	size_type GetValueTypeHash(ValueType const& Value, size_type Num) const
	{
		return GetKeyTypeHash(ExtractKey(Value), Num);
	}

	size_type GetKeyTypeHash(KeyType const& Key, size_type Num) const
	{
		return GetTypeHash(Key) % Num;
	}

	std::pair<iterator, bool> insert_unique_noresize(ValueType const& Value);
	iterator insert_equal_noresize(ValueType const& Value);

public:

	size_type bucket_count() const { return Buckets.size(); }

	std::pair<iterator, bool> insert_unique(ValueType const& Value)
	{
		resize(ElementsNum + 1);
		return insert_unique_noresize(Value);
	}

	iterator insert_equal(ValueType const& Value)
	{
		resize(ElementsNum + 1);
		return insert_equal_noresize(Value);
	}

	void resize(size_type ElementsNum);
	void clear();

	void copy_from(HashTable const& ht);
};

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
std::pair<typename HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::iterator, bool>
HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::insert_unique_noresize(ValueType const& Value)
{
	size_type iBucketIndex = GetValueTypeHash(Value);
	Node* Cur = Buckets[iBucketIndex];

	while (Cur)
	{
		if (EqualKey(ExtractKey(Cur->Value), ExtractKey(Value)))
			return std::pair<iterator, bool>(iterator(Cur, this), false);
		Cur = Cur->Next;
	}
	
	Node* tmpNode = new_node(Value);
	tmpNode->Next = Cur;
	Buckets[iBucketIndex] = tmpNode;
	++ElementsNum;
	return std::pair<iterator, bool>(iterator(Cur, this), true);
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
typename HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::iterator
HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::insert_equal_noresize(ValueType const& Value)
{
	size_type const iBucketIndex = GetValueTypeHash(Value);
	Node* Cur = Buckets[iBucketIndex];

	while (Cur)
	{
		if (EqualKey(ExtractKey(Cur->Value), ExtractKey(Value)))
		{
			Node* tmpNode = new_node(Value);
			//	insert after Cur pointer
			tmpNode->Next = Cur->Next;
			Cur->Next = tmpNode;
			++ElementsNum;
			return iterator(tmpNode, this);
		}
		Cur = Cur->Next;
	}

	Node* tmpNode = new_node(Value);
	tmpNode->Next = Cur;
	Buckets[iBucketIndex] = tmpNode;
	++ElementsNum;
	return iterator(tmpNode, this);
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
void HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::resize(size_type ElementsNum)
{
	size_type OldBucketsNum = Buckets.size();
	if (ElementsNum > OldBucketsNum)
	{
		size_type NewBucketsNum = next_size(ElementsNum);
		if (NewBucketsNum > OldBucketsNum)
		{
			Array<Node*, Alloc> tmpBuckets(NewBucketsNum, (Node*)0);
			for (size_type iBucketIndex = 0; iBucketIndex < OldBucketsNum; ++iBucketIndex)
			{
				Node* Cur = Buckets[iBucketIndex];
				while (Cur)
				{
					size_type NewBucketIndex = bkt_num(Cur->Value);
					//	insert the head of single bucket
					Buckets[iBucketIndex] = Cur->Next;
					Cur->Next = tmpBuckets[NewBucketIndex];
					tmpBuckets[NewBucketIndex] = Cur;
					Cur = Buckets[iBucketIndex];
				}
				Buckets.swap(tmpBuckets);
			}
		}
	}
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
void HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::clear()
{
	for (size_type i = 0; i < Buckets.size(); ++i)
	{
		Node* Cur = Buckets[i];
		while (Cur)
		{
			Node* Next = Cur->Next;
			delete_node(Cur);
			Cur = Next;
		}
		Buckets[i] = 0;
	}
	ElementsNum = 0;
}

template <typename ValueType, typename KeyType, typename HashFunction, typename ExtractKeyType, typename EqualKeyType, typename Alloc>
void HashTable<ValueType, KeyType, HashFunction, ExtractKeyType, EqualKeyType, Alloc>::copy_from(HashTable const& ht)
{
	Buckets.clear();

	Buckets.reserve(ElementsNum);
	Buckets.insert(Buckets.end(), ht.Buckets.size(), (Node*)0);

	for (size_type i = 0; i < ht.Buckets.size(); ++i)
	{
		if (Node const* Cur = ht.Buckets[i])
		{
			Node* NewCur = new_node(Cur->Value);
			Buckets[i] = NewCur;

			for (Node* Next = Cur->Next; Next; Cur = Next, Next = Cur->Next)
			{
				NewCur->Next = new_node(Next->Value);
				NewCur = NewCur->Next;
			}
		}
	}
	ElementsNum = ht.ElementsNum;
}