#pragma once

#include "SGISTL/Deque.h"

template <typename T, typename Sequence = deque<T>>
class Stack
{
	friend bool operator == (Stack const&, Stack const&);
	friend bool operator < (Stack const&, Stack const&);

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

	Stack() : c() {}
	explicit Stack(Sequence const& s) : c(s) {}

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(value_type const& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
};

template <typename T, typename Sequence>
bool operator == (Stack<T, Sequence> const& lhs, Stack<T, Sequence> const& rhs)
{
	return lhs.c == rhs.c;
}

template <typename T, typename Sequence>
bool operator < (Stack<T, Sequence> const& lhs, Stack<T, Sequence> const& rhs)
{
	return lhs.c < rhs.c;
}
