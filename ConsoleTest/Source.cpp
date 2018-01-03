#include <iostream>

#include "Template/IdentityFunctor.h"
//#include "SGISTL/Allocator.h"

#include "Template/IsTrivially.h"
#include <vector>

#include <type_traits> 
#include <iostream> 

#include "SGISTL/Construct.h"
#include "Template/AndOrNot.h"

#include "SGISTL/Vector.h"
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

	//IdentityFunctor i;

	//std::cout << i(1) << std::endl;


	int ia[5] = { 0,1,2,3,4 };
	//unsigned int ii;

	//std::vector<int, simple_alloc<int>> iv(ia, ia + 5);

	vector<int> I(10, 33);

	I.pop_back();
	I.push_back(34);

	for (int i = 0; i < I.size(); i++)
	{
		std::cout << I[i] << std::endl;
	}


	int* p = new int;

	memset(p, 33, sizeof(int) * 10);

	std::uninitialized_fill_n(p, 10, 33);


	for (int i = 0; i < 10; i++)
	{
		std::cout << p[i] << std::endl;
	}

	system("pause");

	return 0;
}