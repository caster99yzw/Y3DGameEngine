#include <iostream>

#include "IdentityFunctor.h"

int main()
{

	FIdentityFunctor i;

	std::cout << i(1) << std::endl;

	system("pause");

	return 0;
}