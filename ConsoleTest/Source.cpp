#include <iostream>

#include "Template/IdentityFunctor.h"
//#include "SGISTL/Allocator.h"

#include "Template/IsTriviallyDestructible.h"
#include "Template/IsTriviallyConstructible.h"
#include "Template/IsTriviallyAssignable.h"
#include <vector>

#include <type_traits> 
#include <iostream> 

#include "SGISTL/Construct.h"
#include "Template/AndOrNot.h"

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


	std::cout << "has_trivial_destructor<trivial> == " << std::boolalpha
		<< std::is_trivially_copy_constructible<int>::value << std::endl;

	std::cout << IsTriviallyCopyConstructible<int>::Value << std::endl;


	std::cout << "has_trivial_destructor<trivial> == " << std::boolalpha
		<< std::is_trivially_copy_assignable<int>::value << std::endl;

	std::cout << IsTriviallyCopyAssignable<int>::Value << std::endl;

	system("pause");

	return 0;
}