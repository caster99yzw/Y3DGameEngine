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

	l.erase((++(++l.begin())));

	for (list<int>::iterator itor = l.begin();
		itor != l.end();
		itor++)
	{
		std::cout << *itor << std::endl;
	}

	system("pause");

	return 0;
}