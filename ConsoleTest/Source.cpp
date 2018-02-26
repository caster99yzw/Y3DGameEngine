#include <iostream>

#include "Template/IdentityFunctor.h"
//#include "SGISTL/Allocator.h"

#include "Template/UniquePtr.h"

#include "Template/IsTrivially.h"
#include <vector>

#include <type_traits> 
#include <iostream> 

#include "SGISTL/Construct.h"
#include "Template/AndOrNot.h"

#include "SGISTL/Vector.h"
#include "SGISTL/List.h"
#include <memory>

int main()
{

	struct trivial
	{
		int val;
		int*  dsss;

	};

	enum XXI
	{

	};

	std::is_enum<XXI>::value;


	vector<int> v;
	list<int> l;

	l.insert(l.begin(), 20);
	l.insert(l.end(), 30);
	l.insert(l.begin(), 50);
	l.insert(((++l.begin())++), 60);

	l.insert(((++l.begin())++), 4, 600000);

	l.push_back(8000);
	l.push_front(8000);

	l.pop_back();
	l.pop_front();

	for (list<int>::iterator itor = l.begin();
		itor != l.end();
		itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;


	list<int> newl(l);

	newl.reverse();

	for (list<int>::iterator itor = newl.begin();
		itor != newl.end();
		itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;


	system("pause");

	return 0;
}