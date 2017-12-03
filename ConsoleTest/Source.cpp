#include <iostream>

#include "Template/IdentityFunctor.h"
#include "Allocator.h"
#include <vector>

int main()
{

	IdentityFunctor i;

	std::cout << i(1) << std::endl;


	int ia[5] = { 0,1,2,3,4 };
	//unsigned int ii;

	//std::vector<int, simple_alloc<int>> iv(ia, ia + 5);


	system("pause");

	return 0;
}