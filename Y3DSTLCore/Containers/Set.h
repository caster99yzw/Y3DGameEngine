#pragma once

#include "SGISTL/RBTree.h"
#include "Template/Less.h"
#include "Template/IdentityFunctor.h"

/*
* Unique Insert
*/
template <typename KeyType, typename CompareType = Less<KeyType>, typename Alloc = alloc>
class set
{
public:

	typedef KeyType				ValueType;
	typedef CompareType			KeyCompare;
	typedef CompareType			ValueCompare;

private:

	typedef RBTree<KeyType, ValueType, IdentityFunctor, KeyCompare, Alloc> RepType;
	RepType	SetImpl;

public:

	typedef typename RepType::const_pointer			pointer;
	typedef typename RepType::const_pointer			const_point;
	typedef typename RepType::const_reference		reference;
	typedef typename RepType::const_reference		const_reference;
	typedef typename RepType::const_iterator		iterator;
	typedef typename RepType::const_iterator		const_iterator;
	typedef typename RepType::size_type				size_type;
	typedef typename RepType::difference_type		difference_type;

	set() : SetImpl(CompareType()) {}
	explicit set(CompareType const& Comp) : SetImpl(Comp) {}

	template <typename InputIterator>
	set(InputIterator First, InputIterator Last)
		: SetImpl(CompareType())
	{
		SetImpl.insert_unique(First, Last);
	}

	template <typename InputIterator>
	set(InputIterator First, InputIterator Last, CompareType const& Comp)
		: SetImpl(Comp)
	{
		SetImpl.insert_unique(First, Last);
	}

	set(set<KeyType, CompareType, Alloc> const& other) : SetImpl(other.SetImpl) {}
	set<KeyType, CompareType, Alloc>& operator = (set<KeyType, CompareType, Alloc> const& other)
	{
		SetImpl = other.SetImpl;
		return *this;
	}

	KeyCompare key_compare() const { return SetImpl.key_compare(); }
	ValueCompare value_compare() const { return SetImpl.key_compare(); }
	//allocator_type get_allocator() const { return SetImpl.get_allocator(); }

	iterator begin() const { return SetImpl.begin(); }
	iterator end() const { return SetImpl.end(); }
	bool empty() const { return SetImpl.empty(); }
	size_type size() const { return SetImpl.size(); }

	void Swap(set<KeyType, CompareType, Alloc>& other) { SetImpl.Swap(other.SetImpl); }

	std::pair<iterator, bool> insert(ValueType const& Value)
	{
		std::pair<typename RepType::iterator, bool> pair = SetImpl.insert_unique(Value);
		return std::pair<iterator, bool>(pair.first, pair.second);
	}

	template <typename InputIterator>
	void insert(InputIterator First, InputIterator Last)
	{
		SetImpl.insert_unique(First, Last);
	}

	void erase(iterator Position)
	{
		typedef typename RepType::iterator RepIterator;
		SetImpl.erase((RepIterator&)Position);
	}

	size_type erase(KeyType const& Value)
	{
		return SetImpl.erase(Value);
	}

	void erase(iterator First, iterator Last)
	{
		typedef typename RepType::iterator RepIterator;
		SetImpl.erase((RepIterator&)First, (RepIterator&)Last);
	}
	void clear() { SetImpl.clear(); }

	iterator find(KeyType const& Value) const { return SetImpl.find(Value); }
	size_type count(KeyType const& Value) const 
	{
		return SetImpl.find(Value) == SetImpl.end() ? 0 : 1;
	}
	iterator lower_bound(KeyType const& Value) const 
	{
		return SetImpl.lower_bound(Value);
	}
	iterator upper_bound(KeyType const& Value) const
	{
		return SetImpl.upper_bound(Value);
	}
	std::pair<iterator, iterator> equal_range(KeyType const& Value) const
	{
		return SetImpl.equal_range(Value);
	}

	friend bool operator == (set const&, set const&);
	friend bool operator < (set const&, set const&);
};

template <typename KeyType, typename CompareType, typename Alloc>
FORCEINLINE bool operator == (set<KeyType, CompareType, Alloc> const& lhs, set<KeyType, CompareType, Alloc> const& rhs)
{
	return lhs.SetImpl == rhs.SetImpl;
}

template <typename KeyType, typename CompareType, typename Alloc>
FORCEINLINE bool operator < (set<KeyType, CompareType, Alloc> const& lhs, set<KeyType, CompareType, Alloc> const& rhs)
{
	return lhs.SetImpl < rhs.SetImpl;
}

/*
* Equal Insert
*/
template <typename KeyType, typename CompareType = Less<KeyType>, typename Alloc = alloc>
class multiset
{
public:

	typedef KeyType				ValueType;
	typedef CompareType			KeyCompare;
	typedef CompareType			ValueCompare;

private:

	typedef RBTree<KeyType, ValueType, IdentityFunctor, KeyCompare, Alloc> RepType;
	RepType	SetImpl;


public:

	typedef typename RepType::const_pointer			pointer;
	typedef typename RepType::const_pointer			const_point;
	typedef typename RepType::const_reference		reference;
	typedef typename RepType::const_reference		const_reference;
	typedef typename RepType::const_iterator		iterator;
	typedef typename RepType::const_iterator		const_iterator;
	typedef typename RepType::size_type				size_type;
	typedef typename RepType::difference_type		difference_type;

	multiset() : SetImpl(CompareType()) {}
	explicit multiset(CompareType const& Comp) : SetImpl(Comp) {}

	template <typename InputIterator>
	multiset(InputIterator First, InputIterator Last)
		: SetImpl(CompareType())
	{
		SetImpl.insert_equal(First, Last);
	}

	template <typename InputIterator>
	multiset(InputIterator First, InputIterator Last, CompareType const& Comp)
		: SetImpl(Comp)
	{
		SetImpl.insert_equal(First, Last);
	}

	multiset(multiset<KeyType, CompareType, Alloc> const& other) : SetImpl(other.SetImpl) {}
	set<KeyType, CompareType, Alloc>& operator = (multiset<KeyType, CompareType, Alloc> const& other)
	{
		SetImpl = other.SetImpl;
		return *this;
	}

	KeyCompare key_compare() const { return SetImpl.key_compare(); }
	ValueCompare value_compare() const { return SetImpl.key_compare(); }
	//allocator_type get_allocator() const { return SetImpl.get_allocator(); }

	iterator begin() const { return SetImpl.begin(); }
	iterator end() const { return SetImpl.end(); }
	bool empty() const { return SetImpl.empty(); }
	size_type size() const { return SetImpl.size(); }

	void Swap(set<KeyType, CompareType, Alloc>& other) { SetImpl.Swap(other.SetImpl); }

	std::pair<iterator, bool> insert(ValueType const& Value)
	{
		std::pair<typename RepType::iterator, bool> pair = SetImpl.insert_equal(Value);
		return std::pair<iterator, bool>(pair.first, pair.second);
	}

	template <typename InputIterator>
	void insert(InputIterator First, InputIterator Last)
	{
		SetImpl.insert_equal(First, Last);
	}

	void erase(iterator Position)
	{
		typedef typename RepType::iterator RepIterator;
		SetImpl.erase((RepIterator&)Position);
	}

	size_type erase(KeyType const& Value)
	{
		return SetImpl.erase(Value);
	}

	void erase(iterator First, iterator Last)
	{
		typedef typename RepType::iterator RepIterator;
		SetImpl.erase((RepIterator&)First, (RepIterator&)Last);
	}
	void clear() { SetImpl.clear(); }

	iterator find(KeyType const& Value) const { return SetImpl.find(Value); }
	size_type count(KeyType const& Value) const
	{
		return SetImpl.find(Value) == SetImpl.end() ? 0 : 1;
	}
	iterator lower_bound(KeyType const& Value) const
	{
		return SetImpl.lower_bound(Value);
	}
	iterator upper_bound(KeyType const& Value) const
	{
		return SetImpl.upper_bound(Value);
	}
	std::pair<iterator, iterator> equal_range(KeyType const& Value) const
	{
		return SetImpl.equal_range(Value);
	}

	friend bool operator == (multiset const&, multiset const&);
	friend bool operator < (multiset const&, multiset const&);
};

template <typename KeyType, typename CompareType, typename Alloc>
FORCEINLINE bool operator == (multiset<KeyType, CompareType, Alloc> const& lhs, multiset<KeyType, CompareType, Alloc> const& rhs)
{
	return lhs.SetImpl == rhs.SetImpl;
}

template <typename KeyType, typename CompareType, typename Alloc>
FORCEINLINE bool operator < (multiset<KeyType, CompareType, Alloc> const& lhs, multiset<KeyType, CompareType, Alloc> const& rhs)
{
	return lhs.SetImpl < rhs.SetImpl;
}