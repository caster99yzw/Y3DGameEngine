#include "stl/Template/IdentityFunctor.h"
#include "stl/Template/UniquePtr.h"
#include "stl/Template/Less.h"
#include "stl/Template/IsTrivially.h"
#include "stl/Template/Invoke.h"
#include "stl/Template/AndOrNot.h"
#include "stl/Containers/Allocator.h"
#include "stl/Containers/Deque.h"
#include "stl/Containers/Vector.h"
#include <vector>
#include <type_traits> 
#include <iostream> 
#include <memory>
//#include "SGISTL/Allocator.h"

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


	vector<int> v;
	v.push_back(12);
	std::cout << v[0];
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
	//priority_queue<int> ipq(ia, ia + 9);

	//for (int i = 0; i < ipq.size(); i++)
	//{
	//	std::cout << ipq.top()<< " ";
	//}
	//std::cout << std::endl;

	//while (!ipq.empty())
	//{
	//	std::cout << ipq.top() << " ";
	//	ipq.pop();
	//}
	//std::cout << std::endl;

	system("pause");

	return 0;
}