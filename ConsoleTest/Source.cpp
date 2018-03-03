#include <iostream>

#include "Template/IdentityFunctor.h"
//#include "SGISTL/Allocator.h"

#include "Template/UniquePtr.h"
#include "Template/Less.h"
#include "Template/IsTrivially.h"
#include "Template/Invoke.h"
#include <vector>

#include <type_traits> 
#include <iostream> 

#include "SGISTL/Construct.h"
#include "Template/AndOrNot.h"

#include "SGISTL/Vector.h"
#include "SGISTL/List.h"
#include "SGISTL/Deque.h"
#include "SGISTL/Heap.h"
#include <memory>

int compare(int a)
{
	return a;
}

struct FIdentityFunctor
{
	template <typename T>
	FORCEINLINE T&& operator()(T&& Val) const
	{
		return (T&&)Val;
	}
};

int main()
{

	//struct trivial
	//{
	//	int val;
	//	int*  dsss;

	//};

	//enum XXI
	//{

	//};

	//std::is_enum<XXI>::value;


	//vector<int> v;
	//list<int> l;

	//l.insert(l.begin(), 20);
	//l.insert(l.end(), 30);
	//l.insert(l.begin(), 50);
	//l.insert(((++l.begin())++), 60);

	//l.insert(((++l.begin())++), 4, 600000);

	//l.push_back(8000);
	//l.push_front(8000);

	//l.pop_back();
	//l.pop_front();

	//for (list<int>::iterator itor = l.begin();
	//	itor != l.end();
	//	itor++)
	//{
	//	std::cout << *itor << " ";
	//}
	//std::cout << std::endl;


	//list<int> newl(l);

	//newl.reverse();

	//for (list<int>::iterator itor = newl.begin();
	//	itor != newl.end();
	//	itor++)
	//{
	//	std::cout << *itor << " ";
	//}
	//std::cout << std::endl;

	int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
	vector<int> ivec(ia, ia + 9);

	for (vector<int>::iterator itor = ivec.begin();
		itor != ivec.end();
		itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;

	MakeHeap(ivec.begin(), ivec.end(), Less<int>());
	ivec.push_back(7);

	PushHeap(ivec.begin(), ivec.end(), Less<int>());

	PopHeap(ivec.begin(), ivec.end(), Less<int>());

	for (vector<int>::iterator itor = ivec.begin();
		itor != ivec.end();
		itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;

	system("pause");

	return 0;
}