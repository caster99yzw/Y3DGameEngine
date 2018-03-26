#pragma once

struct input_iterator_tag
{
};

struct output_iterator_tag
{
};

struct forward_iterator_tag : public input_iterator_tag
{
};

struct bidirectional_iterator_tag : public forward_iterator_tag
{
};

struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};

template <typename _Category,
		  typename _Tp,
		  typename _Distance = ptrdiff_t,
		  typename _Pointer = _Tp*,
		  typename _Reference = _Tp&>
struct iterator
{
	typedef _Category							iterator_category;
	typedef _Tp									value_type;
	typedef _Distance							difference_type;
	typedef _Pointer							pointer;
	typedef _Reference							reference;
};

template <typename _Iterator>
struct iterator_traits
{
	typedef typename random_access_iterator_tag			iterator_category;
	typedef typename _Iterator							value_type;
	typedef typename ptrdiff_t							difference_type;
	typedef typename _Iterator*							pointer;
	typedef typename _Iterator&							reference;
};

template <typename _Iterator>
struct iterator_traits<_Iterator*>
{
	typedef typename random_access_iterator_tag			iterator_category;
	typedef typename _Iterator							value_type;
	typedef typename ptrdiff_t							difference_type;
	typedef typename _Iterator const*					pointer;
	typedef typename _Iterator const&					reference;
};

template <typename _Iterator>
struct iterator_traits<_Iterator const*>
{
	typedef typename _Iterator::iterator_category		iterator_category;
	typedef typename _Iterator::value_type				value_type;
	typedef typename _Iterator::difference_type			difference_type;
	typedef typename _Iterator::pointer					pointer;
	typedef typename _Iterator::reference				reference;
};

template <typename _Iterator>
typename iterator_traits<_Iterator>::iterator_category
iterator_category(_Iterator const&)
{
	typedef typename iterator_traits<_Iterator>::iterator_category _Category;
	return _Category();
}

template <class _Iterator>
inline typename iterator_traits<_Iterator>::difference_type*
distance_type(const _Iterator&)
{
	return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
}

template <class _Iterator>
inline typename iterator_traits<_Iterator>::value_type*
value_type(const _Iterator&)
{
	return static_cast<typename iterator_traits<_Iterator>::value_type*>(0);
}

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag)
{
	while (n--) ++i;
}

template <typename BidiectionalIterator, typename Distance>
inline void __advance(BidiectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}

template <typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
{
	i += n;
}

template <typename InputIterator, typename Distance>
inline void advance(InputIterator& __i, Distance __n) 
{
	__advance(__i, __n, iterator_category(__i));
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
	inline iterator_traits<InputIterator>::difference_type n = 0;

	while (first != last) 
	{
		++first; ++n;
	}
	return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}
