#pragma once

/*
* Unique Insert
*/
template <typename KeyType, typename ValueType, typename CompareType = Less<KeyType>, typename Alloc = alloc>
class map
{
public:

	typedef std::pair<KeyType const, ValueType>		NodeType;
	typedef CompareType								KeyCompare;

private:

	/* 
	*  Attention!! 
	*  The value type of red and black tree is std::pair<KeyType const, ValueType>
	*  Not ValueType
	*/
	typedef RBTree<KeyType, NodeType, IdentityFunctor, KeyCompare, Alloc> RepType;
	RepType	MapImpl;

public:

	typedef typename RepType::pointer				pointer;
	typedef typename RepType::const_pointer			const_point;
	typedef typename RepType::reference				reference;
	typedef typename RepType::const_reference		const_reference;
	typedef typename RepType::iterator				iterator;
	typedef typename RepType::const_iterator		const_iterator;
	typedef typename RepType::size_type				size_type;
	typedef typename RepType::difference_type		difference_type;

	map() : MapImpl(CompareType()) {}
	explicit map(CompareType const& Comp) : MapImpl(Comp) {}

	template <typename InputIterator>
	map(InputIterator First, InputIterator Last)
		: MapImpl(CompareType())
	{
		MapImpl.insert_unique(First, Last);
	}

	template <typename InputIterator>
	map(InputIterator First, InputIterator Last, CompareType const& Comp)
		: MapImpl(Comp)
	{
		MapImpl.insert_unique(First, Last);
	}

	map(map<KeyType, ValueType, CompareType, Alloc> const& other) : MapImpl(other.MapImpl) {}
	map<KeyType, ValueType, CompareType, Alloc>& operator = (map<KeyType, ValueType, CompareType, Alloc> const& other)
	{
		MapImpl = other.MapImpl;
		return *this;
	}

	KeyCompare key_compare() const { return MapImpl.key_compare(); }
	//ValueCompare value_compare() const { return MapImpl.key_compare(); }
	//allocator_type get_allocator() const { return MapImpl.get_allocator(); }

	iterator begin() { return MapImpl.begin(); }
	const_iterator begin() const { return MapImpl.begin(); }
	iterator end() { return MapImpl.end(); }
	const_iterator end() const { return MapImpl.end(); }
	bool empty() const { return MapImpl.empty(); }
	size_type size() const { return MapImpl.size(); }

	ValueType& operator [] (KeyType const& Key) 
	{
		return *(insert(value_type(Key, ValueType())).first).second;
	}

	void Swap(map<KeyType, ValueType, CompareType, Alloc>& other) { MapImpl.Swap(other.MapImpl); }

	std::pair<iterator, bool> insert(ValueType const& Value)
	{
		return MapImpl.insert_unique(Value);
	}

	template <typename InputIterator>
	void insert(InputIterator First, InputIterator Last)
	{
		MapImpl.insert_unique(First, Last);
	}

	void erase(iterator Position)
	{
		MapImpl.erase(Position);
	}

	size_type erase(KeyType const& Value)
	{
		return MapImpl.erase(Value);
	}

	void erase(iterator First, iterator Last)
	{
		MapImpl.erase(First, Last);
	}
	void clear() { MapImpl.clear(); }

	iterator find(KeyType const& Value) { return MapImpl.find(Value); }
	const_iterator find(KeyType const& Value) const { return MapImpl.find(Value); }
	size_type count(KeyType const& Value) const
	{
		return MapImpl.count(Value);
	}
	iterator lower_bound(KeyType const& Value) const
	{
		return MapImpl.lower_bound(Value);
	}
	iterator upper_bound(KeyType const& Value) const
	{
		return MapImpl.upper_bound(Value);
	}
	std::pair<iterator, iterator> equal_range(KeyType const& Value)
	{
		return MapImpl.equal_range(Value);
	}
	std::pair<const_iterator, const_iterator> equal_range(KeyType const& Value) const
	{
		return MapImpl.equal_range(Value);
	}

	friend bool operator == (map const&, map const&);
	friend bool operator < (map const&, map const&);
};

template <typename KeyType, typename ValueType, typename CompareType, typename Alloc>
FORCEINLINE bool operator == (map<KeyType, ValueType, CompareType, Alloc> const& lhs, map<KeyType, ValueType, CompareType, Alloc> const& rhs)
{
	return lhs.MapImpl == rhs.MapImpl;
}

template <typename KeyType, typename ValueType, typename CompareType, typename Alloc>
FORCEINLINE bool operator < (map<KeyType, ValueType, CompareType, Alloc> const& lhs, map<KeyType, ValueType, CompareType, Alloc> const& rhs)
{
	return lhs.MapImpl < rhs.MapImpl;
}

/*
* Equal Insert
*/
template <typename KeyType, typename ValueType, typename CompareType = Less<KeyType>, typename Alloc = alloc>
class multimap
{
public:

	typedef std::pair<KeyType const, ValueType>		NodeType;
	typedef CompareType								KeyCompare;

private:

	typedef RBTree<KeyType, ValueType, IdentityFunctor, KeyCompare, Alloc> RepType;
	RepType	MapImpl;

public:

	typedef typename RepType::pointer				pointer;
	typedef typename RepType::const_pointer			const_point;
	typedef typename RepType::reference				reference;
	typedef typename RepType::const_reference		const_reference;
	typedef typename RepType::iterator				iterator;
	typedef typename RepType::const_iterator		const_iterator;
	typedef typename RepType::size_type				size_type;
	typedef typename RepType::difference_type		difference_type;

	multimap() : MapImpl(CompareType()) {}
	explicit multimap(CompareType const& Comp) : MapImpl(Comp) {}

	template <typename InputIterator>
	multimap(InputIterator First, InputIterator Last)
		: MapImpl(CompareType())
	{
		MapImpl.insert_equal(First, Last);
	}

	template <typename InputIterator>
	multimap(InputIterator First, InputIterator Last, CompareType const& Comp)
		: MapImpl(Comp)
	{
		MapImpl.insert_equal(First, Last);
	}

	multimap(multimap<KeyType, ValueType, CompareType, Alloc> const& other) : MapImpl(other.MapImpl) {}
	multimap<KeyType, ValueType, CompareType, Alloc>& operator = (multimap<KeyType, ValueType, CompareType, Alloc> const& other)
	{
		MapImpl = other.MapImpl;
		return *this;
	}

	KeyCompare key_compare() const { return MapImpl.key_compare(); }
	//ValueCompare value_compare() const { return MapImpl.key_compare(); }
	//allocator_type get_allocator() const { return MapImpl.get_allocator(); }

	iterator begin() { return MapImpl.begin(); }
	const_iterator begin() const { return MapImpl.begin(); }
	iterator end() { return MapImpl.end(); }
	const_iterator end() const { return MapImpl.end(); }
	bool empty() const { return MapImpl.empty(); }
	size_type size() const { return MapImpl.size(); }

	ValueType& operator [] (KeyType const& Key)
	{
		return *(insert(value_type(Key, ValueType())).first).second;
	}

	void Swap(multimap<KeyType, ValueType, CompareType, Alloc>& other) { MapImpl.Swap(other.MapImpl); }

	std::pair<iterator, bool> insert(ValueType const& Value)
	{
		return MapImpl.insert_equal(Value);
	}

	template <typename InputIterator>
	void insert(InputIterator First, InputIterator Last)
	{
		MapImpl.insert_equal(First, Last);
	}

	void erase(iterator Position)
	{
		MapImpl.erase(Position);
	}

	size_type erase(KeyType const& Value)
	{
		return MapImpl.erase(Value);
	}

	void erase(iterator First, iterator Last)
	{
		MapImpl.erase(First, Last);
	}
	void clear() { MapImpl.clear(); }

	iterator find(KeyType const& Value) { return MapImpl.find(Value); }
	const_iterator find(KeyType const& Value) const { return MapImpl.find(Value); }
	size_type count(KeyType const& Value) const
	{
		return MapImpl.count(Value);
	}
	iterator lower_bound(KeyType const& Value) const
	{
		return MapImpl.lower_bound(Value);
	}
	iterator upper_bound(KeyType const& Value) const
	{
		return MapImpl.upper_bound(Value);
	}
	std::pair<iterator, iterator> equal_range(KeyType const& Value)
	{
		return MapImpl.equal_range(Value);
	}
	std::pair<const_iterator, const_iterator> equal_range(KeyType const& Value) const
	{
		return MapImpl.equal_range(Value);
	}

	friend bool operator == (multimap const&, multimap const&);
	friend bool operator < (multimap const&, multimap const&);
};

template <typename KeyType, typename ValueType, typename CompareType, typename Alloc>
FORCEINLINE bool operator == (multimap<KeyType, ValueType, CompareType, Alloc> const& lhs, multimap<KeyType, ValueType, CompareType, Alloc> const& rhs)
{
	return lhs.MapImpl == rhs.MapImpl;
}

template <typename KeyType, typename ValueType, typename CompareType, typename Alloc>
FORCEINLINE bool operator < (multimap<KeyType, ValueType, CompareType, Alloc> const& lhs, multimap<KeyType, ValueType, CompareType, Alloc> const& rhs)
{
	return lhs.MapImpl < rhs.MapImpl;
}