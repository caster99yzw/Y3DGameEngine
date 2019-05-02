#pragma once

#include "Containers/Deque.h"
#include "Containers/Array.h"
#include "Algorithms/BinaryHeap.h"
#include "Template/Greater.h"

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

	Sequence	Seq;

public:

	Queue() : Seq() {}
	explicit Queue(Sequence const& s) : Seq(s) {}

	bool empty() const { return Seq.empty(); }
	size_type size() const { return Seq.size(); }
	reference front() { return Seq.front(); }
	const_reference front() const { return Seq.front(); }
	reference back() { return Seq.back(); }
	const_reference back() const { return Seq.back(); }
	void push(value_type const& x) { Seq.push_back(x); }
	void pop() { Seq.pop_front(); }
};

template <typename T, typename Sequence>
bool operator == (Queue<T, Sequence> const& lhs, Queue<T, Sequence> const& rhs)
{
	return lhs.Seq == rhs.Seq;
}

template <typename T, typename Sequence>
bool operator < (Queue<T, Sequence> const& lhs, Queue<T, Sequence> const& rhs)
{
	return lhs.Seq < rhs.Seq;
}

template <typename T, 
		typename Sequence = Array<T>,
		typename Compare = Greater<typename Sequence::value_type>>
class priority_queue
{
public:

	typedef typename Sequence::value_type			value_type;
	typedef typename Sequence::size_type			size_type;
	typedef typename Sequence::reference			reference;
	typedef typename Sequence::const_reference		const_reference;

protected:

	Sequence	Seq;
	Compare		CompareFunc;

public:

	priority_queue() : Seq() {}
	explicit priority_queue(Compare const& Comp) : Seq(), CompareFunc(Comp) {}

	template <typename InputIterator>
	priority_queue(InputIterator First, InputIterator Last, Compare const& Comp)
		: Seq(First, Last), CompareFunc(Comp)
	{
		MakeHeap(Seq.begin(), Seq.end(), CompareFunc);
	}
	template <typename InputIterator>
	priority_queue(InputIterator First, InputIterator Last)
		: Seq(First, Last)
	{
		MakeHeap(Seq.begin(), Seq.end(), CompareFunc);
	}

	bool empty() { return Seq.empty(); }
	size_type size() const { return Seq.size(); }
	const_reference top() const { return Seq.front(); }

	void push(value_type const& x) 
	{
		Seq.push_back(x); 
		PushHeap(Seq.begin(), Seq.end(), CompareFunc);
	}
	void pop()
	{
		PopHeap(Seq.begin(), Seq.end(), CompareFunc);
		Seq.pop_back();
	}
};