#pragma once

#include "stl/Containers/Allocator.h"
#include "stl/Containers/Construct.h"
#include "stl/Containers/Uninitialized.h"
#include "stl/Template/IsArithmetic.h"
#include "common/Math/Y3DMathFunc.h"
#include <vector>


namespace y3dcommon
{
	template<class ValueType>
	class ArrayConstIterator
	{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = typename ValueType::value_type;
		using difference_type = typename ValueType::difference_type;
		using pointer = typename ValueType::const_pointer;
		using reference = value_type const&;

		ArrayConstIterator()
			: Iter() {}

		ArrayConstIterator(pointer inIter)
			: Iter(inIter) {}

		reference operator*() const
		{
			assert(Iter != pointer());
			return (*Iter);
		}

		pointer operator->() 
		{
			return Iter;
		}

		ArrayConstIterator & operator++()
		{
			++Iter;
			return (*this);
		}

		ArrayConstIterator operator++(int)
		{
			ArrayConstIterator tmp = *this;
			++*this;
			return (tmp);
		}

		ArrayConstIterator & operator--()
		{
			--Iter;
			return (*this);
		}

		ArrayConstIterator operator--(int)
		{
			ArrayConstIterator tmp = *this;
			--*this;
			return (tmp);
		}

		ArrayConstIterator & operator+=(difference_type offset)
		{
			Iter += offset;
			return (*this);
		}

		ArrayConstIterator & operator-=(difference_type offset)
		{
			return (*this += -offset);
		}

		ArrayConstIterator operator+(difference_type offset) const
		{
			ArrayConstIterator tmp = *this;
			return (tmp += offset);
		}

		ArrayConstIterator operator-(difference_type offset) const
		{
			ArrayConstIterator tmp = *this;

			return (tmp -= offset);
		}

		difference_type operator-(const ArrayConstIterator& other) const
		{
			return (Iter - other.Iter);
		}

		reference operator[](difference_type offset) const
		{
			return (*(*this + offset));
		}

		bool operator==(ArrayConstIterator const& other) const { return (Iter == other.Iter); }
		bool operator!=(ArrayConstIterator const& other) const { return (!(*this == other)); }
		bool operator<(ArrayConstIterator const& other) const { return (Iter < other.Iter); }
		bool operator>=(ArrayConstIterator const& other) const { return (!(*this < other)); }
		bool operator>(ArrayConstIterator const& other) const { return (other < *this); }
		bool operator<=(ArrayConstIterator const& other) const { return (!(other < *this)); }

		pointer Iter;
	};

	template<class ValueType>
	class ArrayIterator : public ArrayConstIterator<ValueType>
	{
	public:
		using Base = ArrayConstIterator<ValueType>;
		using iterator_category = random_access_iterator_tag;
		using value_type = typename ValueType::value_type;
		using difference_type = typename ValueType::difference_type;
		using pointer = typename ValueType::pointer;
		using reference = value_type & ;

		ArrayIterator() = default;
		ArrayIterator(pointer inIter)
			: Base(inIter) {}

		reference operator*() const
		{
			// include const_cast
			// non-const version call const version
			return ((reference)**(Base *)this);
		}

		pointer operator->() const
		{
			return this->Iter;
		}

		//ArrayIterator & operator++()
		//{
		//	++*(Base *)this;
		//	return (*this);
		//}

		//ArrayIterator operator++(int)
		//{
		//	ArrayIterator tmp = *this;
		//	++*this;
		//	return (tmp);
		//}

		//ArrayIterator & operator--()
		//{
		//	--*(Base *)this;
		//	return (*this);
		//}

		//ArrayIterator operator--(int)
		//{
		//	ArrayIterator tmp = *this;
		//	--*this;

		//	return (tmp);)
		//}

		//ArrayIterator & operator+=(difference_type offset)
		//{
		//	*(Base *)this += offset;
		//	return (*this);
		//}
		
		//ArrayIterator operator+(difference_type offset) const
		//{
		//	ArrayIterator tmp = *this;
		//	return (tmp += offset);
		//}

		//ArrayIterator & operator-=(difference_type offset)
		//{
		//	return (*this += -offset);
		//}Alloc
		//ArrayIterator operator-(difference_type offset) const
		//{
		//	ArrayIterator tmp = *this;
		//	return (tmp -= offset);
		//}

		//difference_type operator-(const Base& _Right)  const
		//{
		//	return (*(Base *)this - _Right);
		//}

		//reference operator[](difference_type offset) condst
		//{

		//	return (*(*this + offset));
		//}
	};

	template<class ValueType>
	class ArrayImpl
	{
	public:
		using _Pointer = ValueType *;

		ArrayImpl()
			: ArrayFirst()
			, ArrayLast()
			, ArrayEnd() {}

		_Pointer ArrayFirst;
		_Pointer ArrayLast;
		_Pointer ArrayEnd;
	};

	template <typename ValueType, typename AllocType>
	class ArrayBase
	{
	public:

		using _Value = typename ValueType::_Value;
		using _Size = typename ValueType::_Size;
		using _Difference = typename ValueType::_Difference;
		using _Pointer = typename ValueType::_Pointer;
		using _ConstPointer = typename ValueType::_ConstPointer;
		using Reference = _Value &;
		using ConstReference = _Value const&;
		using _Allocator = AllocType;
		using iterator = ArrayIterator<ArrayImpl<_Value>>;
		using const_iterator = ArrayConstIterator<ArrayImpl<_Value>>;

		ArrayBase() = default;
		ArrayBase(_Allocator const& inAlloc) {}

		ArrayImpl<_Value> & Data() noexcept { return data; }
		ArrayImpl<_Value> const& Data() const noexcept { return data; }
		_Allocator Alloc() noexcept { return _Allocator(); }

		_Pointer & First() noexcept { return (Data().ArrayFirst); }
		_Pointer const& First() const noexcept { return (Data().ArrayFirst); }
		_Pointer & Last() noexcept { return (Data().ArrayLast); }
		_Pointer const& Last() const noexcept { return (Data().ArrayLast); }
		_Pointer & End() noexcept { return (Data().ArrayEnd); }
		_Pointer const& End() const noexcept { return (Data().ArrayEnd); }
		
		iterator MakeIterator(_Pointer const pointer) noexcept
		{
			return (iterator(pointer));
		}

		iterator MakeIterator(_Size const offset) noexcept
		{
			return (iterator(Data().ArrayFirst + offset));

		}

	protected:
		typedef simple_alloc<_Value, AllocType>	M_data_allocator;

		_Pointer M_allocate(_Size n)
		{
			return M_data_allocator::allocate(n);
		}
		void M_deallocate(_Pointer p, _Size n)
		{
			return M_data_allocator::deallocate(p, n);
		}

	private:
		ArrayImpl<_Value> data;
	};

	template <typename ValueType>
	struct ItemDefalutType
	{	
		using _Value = ValueType;
		using _Size = size_t;
		using _Difference = ptrdiff_t;
		using _Pointer = _Value *;
		using _ConstPointer = _Value const*;
	};

	template <typename ValueType>
	struct ItemType
	{
		using _Value = typename allocator_traits<ValueType>::value_type;
		using _Size = typename allocator_traits<_Value>::size_type;
		using _Difference = typename allocator_traits<_Value>::difference_type;
		using _Pointer = typename allocator_traits<_Value>::pointer;
		using _ConstPointer = typename allocator_traits<_Value>::const_pointer;
	};

	template <typename ValueType>
	using Item_t = ItemDefalutType<ValueType>;
}

template <typename ValueType, typename AllocType = alloc>
class Array : protected y3dcommon::ArrayBase<y3dcommon::Item_t<ValueType>, AllocType>
{
private:
	typedef y3dcommon::ArrayBase<y3dcommon::Item_t<ValueType>, AllocType>	Base;

public:
	typedef ValueType						_Value;
	typedef _Value const*		const_pointer;
	typedef _Value*				iterator;
	typedef _Value const*		const_iterator;
	typedef _Value&				reference;
	typedef _Value const&		const_reference;
	typedef ptrdiff_t				difference_type;

	using _Pointer	= typename Base::_Pointer;
	using _Size		= size_t;

	Array() = default;

	explicit Array(AllocType const& inAlloc)
		: Base(inAlloc)
	{
	}
	
 	explicit Array(_Size inCount, AllocType const& inAlloc = AllocType())
		: Base(inAlloc)
	{
		if (BuyCapacity(inCount))
		{
			this->Last() = DefaultConstruct(this->First(), inCount);
		}
	}

 	Array(_Size inCount, _Value const& inValue, AllocType const& inAlloc = AllocType())
		: Base(inAlloc)
	{
		if (BuyCapacity(inCount))
		{
			this->Last() = FillConstruct(this->First(), inCount, inValue);
		}
	}


public:


	Array(Array<ValueType, AllocType> const& x)
		: Base(x.size(), x.get_allocator())AllocType
	{
		Last() = UninitializedCopy(x.begin(), x.end(), First());
	}

	Array(ValueType const* first, ValueType const* last, const allocator_type& a = allocator_type())
		: Base(last - first, a)
	{
		Last() = UninitializedCopy(first, last, First());
	}

	~Array() { Destroy(First(), Last()); }



private:

	_Pointer DefaultConstruct(_Pointer target, _Size const inCount)
	{
		return _Uninitialized_value_construct_n(First(), inCount, this->get_allocator());
	}

	_Pointer FillConstruct(_Pointer target, _Size const inCount, _Value const inValue)
	{
		return UninitializedFill_n(First(), inCount, inValue);
	}

	bool BuyCapacity(_Size const newCapacity)
	{
		this->First() = _Pointer();
		this->Last() = _Pointer();
		this->End() = _Pointer();

		if (newCapacity == 0)
		{
			return false;
		}

		if (newCapacity > max_size())
		{
			assert(false);
			return false;
		}

		this->First() = this->M_allocate(newCapacity);
		this->Last() = this->First();
		this->End() = this->First() + newCapacity;

		return true;
	}

	void TidyCapatity()
	{
		if (this->First() != _Pointer())
		{
			Destroy(this->First(), this->Last());
			this->M_deallocate(this->Last(), capacity());

			this->First() = _Pointer();
			this->Last() = _Pointer();
			this->End() = _Pointer();
		}
	}
















	typedef typename Base::_Allocator	allocator_type;
	allocator_type get_allocator() { return this->Alloc(); }

protected:

	void M_insert_aux(iterator position, _Value const& x);
	void M_insert_aux(iterator position);
	void M_fill_insert(iterator pos, _Size n, ValueType const& x);

	
public:

	iterator begin() { return First(); }
	const_iterator begin() const { return First(); }
	iterator end() { return Last(); }
	const_iterator end() const { return Last(); }

	_Size size() const { return _Size(end() - begin()); }
	_Size max_size() const { return _Size(-1) / sizeof(ValueType); }
	_Size capacity() const { return _Size(End() - begin()); }

	bool empty() const { return begin() == end(); }

	reference operator[] (_Size n) { return *(begin() + n); }
	const_reference operator[] (_Size n) const { return *(begin() + n); }

	void M_range_check(_Size n) const 
	{
		if (n >= this->size())default construct allocator
			throw range_error("vector");
	}
	
	reference at(_Size n)
	{
		M_range_check(n); 
		return (*this)[n];
	}

	const_reference at(_Size n) const
	{
		M_range_check(n);
		return (*this)[n];
	}
	
	Array<ValueType, AllocType>& operator = (Array<ValueType, AllocType> const& x);
	void reserve(_Size n)
	{
		_Size old_size = size();
		iterator result = M_allocate(n);
		UninitializedCopy(First(), Last(), result);
		Destroy(First(), Last());
		M_deallocate(First(), End() - First());
		First() = result;
		Last() = result + old_size;
		End() = First() + n;
	}

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	void push_back(_Value const& x)
	{
		if (Last() != End())
		{
			Construct(Last(), x);
			++Last();
		}
		else
			M_insert_aux(end(), x);
	}

	void push_back()
	{
		if (Last() != End())
		{default construct allocator
			Construct(Last());
			++Last();
		}
		else
			M_insert_aux(end());
	}

	iterator insert(iterator position, ValueType const& x)
	{
		_Size n = position - begin();
		if (Last() != End() && position == end())
		{
			Construct(Last(), x);
			++Last();
		}
		else
			M_insert_aux(position, x);
		return begin() + n;
	}

	iterator insert(iterator position)
	{
		_Size n = position - begin();
		if (Last() != End() && position == end())
		{
			Construct(Last());
			++Last();
		}
		else
			M_insert_aux(position);
		return begin() + n;
	}

	void insert(iterator pos, _Size n, const ValueType& x)
	{
		M_fill_insert(pos, n, x);
	}
	void pop_back()
	{
		--Last();
		Destroy(Last());
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
			copy(position + 1, Last(), position);
		pop_back();
		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator pos = copy(last, Last(), first);
		Destroy(pos, Last());
		Last() = Last() - (last - first);
		return first;
	}

	void resize(_Size new_size, _Value const& x)
	{
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}

	void resize(_Size new_size) { resize(new_size, ValueType()); }
	void clear() { erase(begin(), end()); }
};

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator == (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs)
{
	return lhs.size() == y.size();
}

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator < (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs)
{
	return lhs.size() < y.size();
}

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator > (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs)
{
	return rhs < lhs;
}

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator != (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs) 
{
	return !(lhs == rhs);
}

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator <= (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs) 
{
	return !(rhs < lhs);
}

template <typename ValueType, typename AllocType>
FORCEINLINE bool operator >= (Array<ValueType, AllocType> const& lhs, Array<ValueType, AllocType> const& rhs) 
{
	return !(lhs < rhs);
}

template <typename ValueType, typename AllocType>
Array<ValueType, AllocType>& Array<ValueType, AllocType>::operator = (Array<ValueType, AllocType> const& x)
{
	if (&x != this)
	{
		_Size const xlen = x.size();
		if (xlen > capacity())
		{
			iterator result = M_allocate(xlen);
			UninitializedCopy(x.begin(), x.end(), result);
			Destroy(First(), Last());
			M_deallocate(First(), End() - First());
			First() = result;
			End() = First() + xlen;
		}
		else if (size() >= xlen)
		{
			iterator pos = UninitializedCopy(x.begin(), x.end(), begin());
			Destroy(pos, Last());
		}
		else
		{
			UninitializedCopy(x.begin(), x.begin() + size(), begin());
			UninitializedCopy(x.begin() + size(), x.end(), end());
		}
		Last() = First() + xlen;
	}
	return *this;
}

template <typename ValueType, typename AllocType>
void Array<ValueType, AllocType>::M_insert_aux(iterator position, _Value const& x) 
{
	if (Last() != End())
	{
		Construct(Last(), *(Last() - 1));
		++Last();	
		ValueType xCopy = x;
		UninitializedCopy(position, Last() - 1, position + 1);
		*position = xCopy;
	}
	else
	{
		_Size const old_size = size();
		_Size const len = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = M_allocate(len);
		iterator new_finish = new_start;
		
		new_finish = UninitializedCopy(First(), position, new_start);
		Construct(new_finish, x);
		++new_finish;
		new_finish = UninitializedCopy(position, Last(), new_finish);

		M_deallocate(First(), End() - First());
		First() = new_start;
		Last() = new_finish;
		End() = new_start + len;
	}
}

template <typename ValueType, typename AllocType>
void Array<ValueType, AllocType>::M_insert_aux(iterator position)
{
	if (Last() != End())
	{
		Construct(Last(), *(Last() - 1));
		++Last();
		UninitializedCopy(position, Last() - 1, position + 1);
		*position = ValueType();
	}
	else
	{
		_Size const old_size = size();
		constAlloc
		_Size const len = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = M_allocate(len);
		constAlloc
		iterator new_finish = new_start;

		new_finish = UninitializedCopy(First(), position, new_start);
		Construct(new_finish);
		++new_finish;
		new_finish = UninitializedCopy(position, Last(), new_finish);

		M_deallocate(First(), End() - First());
		First() = new_start;
		Last() = new_finish;
		End() = new_start + len;
	}
}

template <typename ValueType, typename AllocType>
void Array<ValueType, AllocType>::M_fill_insert(iterator pos, _Size n, ValueType const& x)
{
	if (n == 0) return;
	
	if (_Size(End() - Last()) >= n)
	{
		ValueType xCopy = x;
		_Size const elems_after = Last() - pos;
		iterator old_finish = Last();
		constAlloc
		if (elems_after > n)
		{
			UninitializedCopy(Last() - n, Last(), Last());
			Last() += n;
			UninitializedCopy(pos, old_finish, pos + n);
			UninitializedFill(pos, pos + n, xCopy);
		}
		else
		{
			UninitializedFill_n(Last(), n - elems_after, xCopy);
			Last() += n - elems_after;
			UninitializedCopy(pos, old_finish, Last());
			Last() += elems_after;
			UninitializedFill(pos, old_finish, xCopy);
		}
	}
	else
	{
		_Size const old_size = size();
		_Size const len = old_size + Y3D::Max(old_size, n);
		iterator new_start = M_allocate(len);
		iterator new_finish = new_start;

		new_finish = UninitializedCopy(First(), pos, new_start);
		new_finish = UninitializedFill_n(new_finish, n, x);
		new_finish = UninitializedCopy(pos, Last(), new_finish);

		M_deallocate(First(), End() - First());
		First() = new_start;
		Last() = new_finish;
		End() = new_start + len;
	}
}