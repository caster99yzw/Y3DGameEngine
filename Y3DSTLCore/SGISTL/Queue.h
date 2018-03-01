#pragma once

#include "SGISTL/Deque.h"

template <typename T, typename Sequence = deque<T>>
class Queue
{
	friend bool operator == (Queue const&, Queue const&);
	friend bool operator < (Queue const&, Queue const&);

	//friend bool operator ==  <T>(Stack const&, Stack const&);
	//friend bool operator < <T> (Stack const&, Stack const&);

public:

	typedef typename Sequence::value_type			value_type;
	typedef typename Sequence::size_type			size_type;
	typedef typename Sequence::reference			reference;
	typedef typename Sequence::const_reference		const_reference;

protected:

	Sequence	c;

public:

	Queue() : c() {}
	explicit Queue(Sequence const& s) : c(s) {}

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(value_type const& x) { c.push_back(x); }
	void pop() { c.pop_front(); }
};

template <typename T, typename Sequence>
bool operator == (Queue<T, Sequence> const& lhs, Queue<T, Sequence> const& rhs)
{
	return lhs.c == rhs.c;
}

template <typename T, typename Sequence>
bool operator < (Queue<T, Sequence> const& lhs, Queue<T, Sequence> const& rhs)
{
	return lhs.c < rhs.c;
}
